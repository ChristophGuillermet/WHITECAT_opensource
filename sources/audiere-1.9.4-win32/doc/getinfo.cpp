#include "audiere.h"
      #include "sndfile.h"
  
  audiere::OutputStream* output_stream2;
  
  //Alot of other code that you don't need to see to help with this problem..

  //Open file, get information about it and then close it.
  SNDFILE* file;
  SF_INFO file_info; //Accessible file information.
  file = sf_open ("ljudfiler/HF01.wav", SFM_READ, &file_info);
 
  if (! file)
    this->angra_message_textbox->Text="Error when using sf_open;file1";
  else 
    {
    file_noise_sample_rate = file_info.samplerate;
    file_noise_channels = file_info.channels;
    file_noise_number_of_frames = file_info.frames;
    sf_close (file);
    //Todo: Error handling.
    } 

  //Open file2, get information about it and then close it.
  SNDFILE* file2; //internal information on file.
  SF_INFO file_info2; //Accessible file information.
  file2 = sf_open ("ljudfiler/noise/cocktail_noise.wav", SFM_READ, &file_info2);
  
  if (!file2)
    this->angra_message_textbox->Text="Error when using sf_open;file2";

  else
    {
  file_sample_rate = file_info2.samplerate;
  file_channels = file_info2.channels;
  file_number_of_frames = file_info2.frames;
  sf_close (file2);
  //Todo: Error handling.
    }

  //Open files and read them into buffer.
  FILE* file_a = fopen ("ljudfiler/HF01.wav", "rb");
  long len;
  short int *buffer;

  if (! file_a)
  this->angra_message_textbox->Text="Fel när filen skulle öppnas...";
  
  else
    { 
    fseek (file_a,0, SEEK_END); //go to end..
    len = ftell (file_a); //get position at end (length)
    fseek (file_a,0,SEEK_SET); //go to beginning
    buffer = (short int*) malloc (len); //malloc buffer
    fread (buffer, len, 1, file_a); //read into buffer
    fclose (file_a);
    }

  FILE* file_b = fopen ("ljudfiler/noise/cocktail_noise.wav", "rb");
  long len2;
  short int *buffer2;
  
  if (!file_b )
  this->angra_message_textbox->Text="Fel när filen skulle öppnas...";
  
  else
    {
    fseek (file_b,0, SEEK_END); //go to end..
    len2 = ftell (file_b); //get position at end (length)
    fseek (file_b,0,SEEK_SET); //go to beginning
    buffer2 = (short int*) malloc (len2); //malloc buffer
    fread (buffer2, len2,1, file_b); //read into buffer
    fclose (file_b);
    }

  //Place in SampleBuffers to be able to mix the samples of both wav files.

  SampleBuffer* sample_buffer = CreateSampleBuffer (buffer,file_number_of_frames,file_channels,file_sample_rate,SF_S16);
  SampleBuffer* sample_buffer2 = CreateSampleBuffer (buffer2,file_noise_number_of_frames,file_noise_channels,file_noise_sample_rate,SF_S16);

  //Open a seekable sample source using the samples contained in the buffer.
  SampleSource* sample_source = sample_buffer->openStream ();
  SampleSource* sample_source2 = sample_buffer2->openStream ();

  sample_source -> read (file_number_of_frames, buffer);
  sample_source2 -> read (file_noise_number_of_frames, buffer2);

  //Now the samples are placed in a buffer and we can manipulate with the buffers.
  //Mix audio file with noise file.

  for (int i =0; i < file_number_of_frames; i++ )
    {
  buffer[i] = buffer[i]+buffer2[i];
    }

                                 //Now we want to play the resulting buffer, that contains samples of the mixed wav-files.
  SampleBuffer* sample_buffer3 = CreateSampleBuffer (buffer,file_number_of_frames,file_channels,file_sample_rate,SF_S16);
  int file_to_play_frames;
  int file_to_play_channels;
  int file_to_play_sample_rate;
  enum SampleFormat file_noise_format = SF_S16;
  AudioDevicePtr audio_device(OpenDevice());
  sample_buffer3->getFormat (file_to_play_channels, file_to_play_sample_rate, file_noise_format);
  output_stream2 = audio_device->AudioDevice::openBuffer (sample_buffer3, file_number_of_frames, file_to_play_channels,file_to_play_sample_rate,SF_S16);
 

  output_stream2->play(); //Play the content of buffer

      Sleep (20000);
  fclose (file_a);
  //Todo: Error handling.
