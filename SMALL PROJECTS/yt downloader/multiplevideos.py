from pytube import YouTube
import os
from pytube import Stream
from pytube.contrib.playlist import Playlist

goe = "https://youtube.com/playlist?list=PLWNDWPAClRVrqQOkoucEQNO4RjZCAbKzv"


# link=open('links_file.txt','r')

link = Playlist(goe)


for i in link:
	# os.system('cls' if os.name == 'nt' else 'clear')
	try:
		yt = YouTube(i)
	except:
		print("Connection Error")
	
	try:
		yt = yt.streams.get_highest_resolution()
	except:
		print("Some Error!")
	print(yt.title," is downloading ......")
	try:
		yt.download()
	except:
		print("time exceed restarting again ....")
