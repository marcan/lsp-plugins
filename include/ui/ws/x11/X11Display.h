/*
 * UICore.h
 *
 *  Created on: 10 окт. 2016 г.
 *      Author: sadko
 */

#ifndef UI_X11_X11DISPLAY_H_
#define UI_X11_X11DISPLAY_H_

#ifndef UI_X11_WS_H_INCL_
    #error "This header should not be included directly"
#endif /* UI_X11_WS_H_INCL_ */

#include <time.h>
#include <data/cvector.h>

namespace lsp
{
    namespace ws
    {
        namespace x11
        {
            class X11Window;
            class X11Clipboard;

            class X11Display: public IDisplay
            {
                friend class X11Window;

                protected:
                    enum x11_async_types
                    {
                        X11ASYNC_CB_RECV,
                        X11ASYNC_CB_SEND,
                        X11ASYNC_DND_RECV
                    };

                    enum x11_cb_recv_states
                    {
                        CB_RECV_CTYPE,
                        CB_RECV_SIMPLE,
                        CB_RECV_INCR
                    };

                    typedef struct wnd_lock_t
                    {
                        X11Window      *pOwner;
                        X11Window      *pWaiter;
                        ssize_t         nCounter;
                    } wnd_lock_t;

                    typedef struct cb_request_t
                    {
                        Atom                hProperty;
                        Atom                hSelection;
                        Time                nTime;
                        X11Clipboard       *pCB;
                        io::IInStream      *pIn;
                        clipboard_handler_t pHandler;
                        void               *pArgument;
                    } cb_request_t;

                    struct x11_async_t;

                    typedef struct cb_common_t
                    {
                        bool                bComplete;
                        Atom                hProperty;
                    } cb_common_t;

                    typedef struct cb_recv_t: public cb_common_t
                    {
                        Atom                hSelection;
                        Atom                hType;
                        x11_cb_recv_states  enState;
                        IDataSink          *pSink;
                    } cb_recv_t;

                    typedef struct cb_send_t: public cb_common_t
                    {
                        Atom                hSelection;
                        Atom                hType;
                        Window              hRequestor;
                        IDataSource        *pSource;
                        io::IInStream      *pStream;
                    } cb_send_t;

                    typedef struct dnd_recv_t: public cb_common_t
                    {
                    } dnd_recv_t;

                    typedef struct x11_async_t
                    {
                        x11_async_types     type;
                        status_t            result;
                        union
                        {
                            cb_common_t         cb_common;
                            cb_recv_t           cb_recv;
                            cb_send_t           cb_send;
                            dnd_recv_t          dnd_recv;
                        };
                    } x11_async_t;

                protected:
                    volatile bool   bExit;
                    Display        *pDisplay;
                    Window          hRootWnd;           // Root window of the display
                    Window          hClipWnd;           // Unmapped clipboard window
                    int             nBlackColor;
                    int             nWhiteColor;
                    x11_atoms_t     sAtoms;
                    Cursor          vCursors[__MP_COUNT];
                    uint8_t        *pIOBuf;
                    IClipboard     *pClipboard[_CBUF_TOTAL];
                    IDataSource    *pCbOwner[_CBUF_TOTAL];

                    cstorage<dtask_t>       sPending;
                    cvector<X11Window>      vWindows;
                    cvector<X11Window>      sGrab;
                    cvector<X11Window>      sTargets;
                    cstorage<wnd_lock_t>    sLocks;
                    cstorage<cb_request_t>  sCbRequests;
                    cstorage<x11_async_t>   sAsync;
                    cvector<char>           vDndMimeTypes;
                    Window                  hDndSource;

                protected:
                    void            handleEvent(XEvent *ev);
                    bool            handleClipboardEvent(XEvent *ev);
                    status_t        do_main_iteration(timestamp_t ts);
                    void            do_destroy();
                    X11Window      *get_locked(X11Window *wnd);
                    X11Window      *get_redirect(X11Window *wnd);
                    static void     compress_long_data(void *data, size_t nitems);
                    Atom            gen_selection_id();
                    cb_request_t   *find_request(Window requestor, Atom selection, Time time);
                    X11Window      *find_window(Window wnd);
                    status_t        bufid_to_atom(size_t bufid, Atom *atom);
                    status_t        atom_to_bufid(Atom x, size_t *bufid);

                    status_t        read_dnd_mime_types(XClientMessageEvent *ev, cvector<char> *ctype);
                    void            drop_mime_types(cvector<char> *ctype);
                    static status_t sink_data_source(IDataSink *dst, IDataSource *src);

                    void            handle_property_notify(XPropertyEvent *ev);
                    status_t        handle_property_notify(cb_recv_t *task, XPropertyEvent *ev);
                    status_t        handle_property_notify(cb_send_t *task, XPropertyEvent *ev);

                    void            handle_selection_notify(XSelectionEvent *ev);
                    status_t        handle_selection_notify(cb_recv_t *task, XSelectionEvent *ev);

                    void            handle_selection_request(XSelectionRequestEvent *ev);
                    status_t        handle_selection_request(cb_send_t *task, XSelectionRequestEvent *ev);

                    status_t        read_property(Window wnd, Atom property, Atom ptype, uint8_t **data, size_t *size, Atom *type);
                    status_t        decode_mime_types(cvector<char> *ctype, const uint8_t *data, size_t size);
                    void            complete_tasks();

                public:
                    explicit X11Display();
                    virtual ~X11Display();

                public:
                    virtual int init(int argc, const char **argv);
                    virtual void destroy();

                    virtual INativeWindow *createWindow();
                    virtual INativeWindow *createWindow(size_t screen);
                    virtual INativeWindow *createWindow(void *handle);
                    virtual INativeWindow *wrapWindow(void *handle);
                    virtual ISurface *createSurface(size_t width, size_t height);

                    virtual int main();
                    virtual status_t main_iteration();
                    virtual void quit_main();

                    virtual size_t screens();
                    virtual size_t default_screen();
                    virtual status_t screen_size(size_t screen, ssize_t *w, ssize_t *h);

                    virtual status_t fetchClipboard(size_t id, const char *ctype, clipboard_handler_t handler, void *arg);
                    virtual status_t writeClipboard(size_t id, IClipboard *c);

                    virtual status_t setClipboard(size_t id, IDataSource *ds);
                    virtual status_t getClipboard(size_t id, IDataSink *dst);

                public:
                    bool                addWindow(X11Window *wnd);
                    bool                remove_window(X11Window *wnd);

                    inline Display             *x11display() const  { return pDisplay; }
                    inline Window               x11root() const     { return hRootWnd; }
                    inline const x11_atoms_t   &atoms() const       { return sAtoms; }
                    Cursor              get_cursor(mouse_pointer_t pointer);

                    size_t              get_screen(Window root);

                    status_t            grab_events(X11Window *wnd);
                    status_t            ungrab_events(X11Window *wnd);

                    status_t            lock_events(X11Window *wnd, X11Window *lock);
                    status_t            unlock_events(X11Window *wnd);

                    virtual void        sync();
                    void                flush();

                public:
                    static const char *event_name(int xev_code);
            };
        }
    }
}

#endif /* UI_X11_X11DISPLAY_H_ */
