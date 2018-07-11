/*
 * ui_ports.h
 *
 *  Created on: 18 мая 2016 г.
 *      Author: sadko
 */

#ifndef CONTAINER_JACK_UI_PORTS_H_
#define CONTAINER_JACK_UI_PORTS_H_

namespace lsp
{
    class JACKUIPort: public IUIPort
    {
        protected:
            JACKPort               *pPort;

        public:
            JACKUIPort(JACKPort *port) : IUIPort(port->metadata())
            {
                pPort       = port;
            }

            virtual ~JACKUIPort()
            {
                pPort       = NULL;
            }

        public:
            virtual bool sync()         { return false; };

            virtual void resync()       { };
    };

    class JACKUIPortGroup: public JACKUIPort
    {
        private:
            JACKPortGroup          *pPG;

        public:
            JACKUIPortGroup(JACKPortGroup *port) : JACKUIPort(port)
            {
                pPG                 = port;
            }

            virtual ~JACKUIPortGroup()
            {
            }

        public:
            virtual float getValue()
            {
                return pPort->getValue();
            }

            virtual void setValue(float value)
            {
                pPort->setValue(value);
            }

        public:
            inline size_t rows() const  { return pPG->rows(); }
            inline size_t cols() const  { return pPG->cols(); }
    };

    class JACKUIControlPort: public JACKUIPort
    {
        protected:
            float           fValue;

        public:
            JACKUIControlPort(JACKPort *port): JACKUIPort(port)
            {
                fValue      = port->getValue();
            }

            virtual ~JACKUIControlPort()
            {
                fValue      = pMetadata->start;
            }

        public:
            virtual float getValue()
            {
                return fValue;
            }

            virtual void setValue(float value)
            {
                fValue  = value;
                static_cast<JACKControlPort *>(pPort)->updateValue(fValue);
            }

            virtual void write(const void *buffer, size_t size)
            {
                if (size == sizeof(float))
                {
                    fValue  = *reinterpret_cast<const float *>(buffer);
                    static_cast<JACKControlPort *>(pPort)->updateValue(fValue);
                }
            }
    };

    class JACKUIMeterPort: public JACKUIPort
    {
        private:
            float   fValue;

        public:
            JACKUIMeterPort(JACKPort *port): JACKUIPort(port)
            {
                fValue      = port->getValue();
            }

            virtual ~JACKUIMeterPort()
            {
                fValue      = pMetadata->start;
            }

        public:
            virtual float getValue()
            {
                return fValue;
            }

            virtual bool sync()
            {
                float value = fValue;

                if (pMetadata->flags & F_PEAK)
                {
                    JACKMeterPort *mp = static_cast<JACKMeterPort *>(pPort);
                    fValue      = mp->syncValue();
                }
                else
                    fValue      = pPort->getValue();
                return fValue != value;
            }
    };

    class JACKUIMeshPort: public JACKUIPort
    {
        private:
            mesh_t      *pMesh;

        public:
            JACKUIMeshPort(JACKPort *port): JACKUIPort(port)
            {
                pMesh       = jack_create_mesh(port->metadata());
            }

            virtual ~JACKUIMeshPort()
            {
                jack_destroy_mesh(pMesh);
                pMesh = NULL;
            }

        public:
            virtual bool sync()
            {
                mesh_t *mesh = reinterpret_cast<mesh_t *>(pPort->getBuffer());
                if ((mesh == NULL) || (!mesh->containsData()))
                    return false;

                // Copy mesh data
                for (size_t i=0; i < mesh->nBuffers; ++i)
                    dsp::copy(pMesh->pvData[i], mesh->pvData[i], mesh->nItems);
                pMesh->data(mesh->nBuffers, mesh->nItems);

                // Clean source mesh
                mesh->cleanup();
                return true;
            }

            virtual void *getBuffer()
            {
                return pMesh;
            }
    };

    class JACKUIPathPort: public JACKUIPort
    {
        private:
            jack_path_t    *pPath;
            char            sPath[PATH_MAX];

        public:
            JACKUIPathPort(JACKPort *port): JACKUIPort(port)
            {
                path_t *path    = reinterpret_cast<path_t *>(pPort->getBuffer());
                if (path != NULL)
                    pPath           = static_cast<jack_path_t *>(path);
                else
                    pPath           = NULL;
                sPath[0]            = '\0';
            }

            virtual ~JACKUIPathPort()
            {
                pPath       = NULL;
            }

        public:
            virtual void *getBuffer()
            {
                return sPath;
            }

            virtual void write(const void *buffer, size_t size)
            {
                // Store path string
                if (size >= PATH_MAX)
                    size = PATH_MAX - 1;
                memcpy(sPath, buffer, size);
                sPath[size] = '\0';

                // Submit path string to DSP
                if (pPath != NULL)
                    pPath->submit(sPath);
            }
    };

}

#endif /* CONTAINER_JACK_UI_PORTS_H_ */