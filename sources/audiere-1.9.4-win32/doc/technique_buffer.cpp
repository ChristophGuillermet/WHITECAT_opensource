
				 audiere::OutputStreamPtr* output_stream2;

                 //Alot of code that you don't need to see in order to help.
				 enum SampleFormat file_noise_format = SF_S16;
				 FILE* file_a = fopen ("ljudfiler/HF01.wav", "rb");
				 long len;
				 short int *buffer;
				 long len2;
				 short int *buffer2;
				 int nr_of_channels =1;
				 int sample_rate2 =44100;
				 

				 if (! file_a)
					 //Error message
				 
				 else
				   { 
				   len=100000*44100*1;
				   buffer = (short int*) malloc (len); //malloc buffer
				   fread (buffer, len, 1, file_a); //read into buffer
				   }

				 FILE* file_b = fopen ("ljudfiler/noise/cocktail_noise.wav", "rb");
				 
				 if (!file_b )
					 //TODO:Error message
				 
				 else
				   {
					 len2=100000*44100*1;
					 buffer2 = (short int*) malloc (len2); //malloc buffer
					 fread (buffer2,len2 ,1, file_b); //read into buffer
				   }

                 SampleBuffer* sample_buffer = CreateSampleBuffer (buffer,100000,1,44100,SF_S16);
				 SampleBuffer* sample_buffer2 = CreateSampleBuffer (buffer2,100000,1,44100,SF_S16);

				 //Open a seekable sample source using the samples contained in the buffer.
				 SampleSource* sample_source = sample_buffer->openStream ();
				 SampleSource* sample_source2 = sample_buffer2->openStream();

				 //The samples are placed in a buffer and we can manipulate with buffers.
				 
				 for (int i =0; i < file_number_of_frames; i++ )
				   {
					 buffer[i] = buffer[i]+buffer2[i];
				   }

				 //In sample_buffer3 we will store what should be played.	
				 SampleBuffer* sample_buffer3 = CreateSampleBuffer (buffer,100000,1,44100,SF_S16);
				 SampleSource* sample_source3 = sample_buffer3->openStream ();
				 
				 AudioDevice* dev;
				 dev = OpenDevice (0, "buffer=10000, rate=44100");
				 
				 if (dev == 0)
					 //TODO: Error message:

				 SampleSourcePtr sample_device_ptr(sample_source3);
				 
				 sample_buffer3->getFormat ((int&)nr_of_channels,(int&)sample_rate2,file_noise_format);
				 
				 //true stands for that the audio file should be played.
				 output_stream2 = new OutputStreamPtr (OpenSound(dev,(const SampleSourcePtr&)sample_device_ptr,true));
