import openai
import os

openai.api_key = "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"

# use the openai api to generate an audio transcription of all the audio files in the audio folder
# and save the transcription to a text file in the transcript folder with the name of the original recording

# get a list of all the audio files in the audio folder
audio_dir = "../audio"
transcript_dir = "../transcripts"
audio_files = os.listdir(audio_dir)
for f in audio_files:
    print("Transcribing " + f + "...")
    audio_file = open(audio_dir + "/" + f, "rb")
    transcript = openai.Audio.transcribe("whisper-1", audio_file)
    tx_name = f.split(".")[0] + ".txt"
    tx_file = open(transcript_dir + "/" + tx_name, "w")
    text = transcript["text"].replace(". ", ".\r")
    tx_file.write(text)
    tx_file.close()
    print("Transcription saved to " + tx_name)

print("Done.")
