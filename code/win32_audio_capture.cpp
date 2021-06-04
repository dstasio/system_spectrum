// win32_audio_capture.cpp
#include <mmdeviceapi.h>
#include <audioclient.h>

#define REFTIMES_PER_SEC  10000000

void
win32_get_system_audio()
{
    CLSID clsid_device_enumerator = __uuidof(MMDeviceEnumerator);
    IID     iid_device_enumerator = __uuidof(IMMDeviceEnumerator);

    IMMDeviceEnumerator *device_enumerator = 0;
    IMMDevice *audio_device = 0;
    IAudioClient *audio_client = 0;
    WAVEFORMATEX *mix_format = 0;
    u32 n_buffer_frames = 0;

    // @todo: INPROC_HANDLER?
    CoCreateInstance(clsid_device_enumerator, 0, CLSCTX_INPROC_HANDLER, iid_device_enumerator, (void **)&device_enumerator);

    if (device_enumerator)
    {
        device_enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &audio_device);
        if (audio_device)
        {
            audio_device->Activate(IID_IAudioClient, CLSCTX_ALL, 0, (void **)&audio_client);

            audio_client->GetMixFormat(&mix_format);
            audio_client->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, REFTIMES_PER_SEC, 0, mix_format, 0);

            audio_client->GetBufferSize(&n_buffer_frames);

        }
        else
        {
            throw_error_and_exit("Could not get default audio endpoint!");
        }
    }
    else
    {
        throw_error_and_exit("Could not get device enumerator!");
    }
}
