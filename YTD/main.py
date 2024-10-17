import os
import yt_dlp
import subprocess
import re
import shlex

def sanitize_filename(filename):
    return re.sub(r'[\\/*?:"<>|]', "", filename)

def get_format_info(format_dict):
    if isinstance(format_dict, dict):
        return {
            'format_id': format_dict.get('format_id', 'unknown'),
            'ext': format_dict.get('ext', 'mp4'),
            'vcodec': format_dict.get('vcodec', 'unknown'),
            'acodec': format_dict.get('acodec', 'unknown'),
            'height': format_dict.get('height', 0)
        }
    return None

def download_video(url, output_path="downloads/single_videos",quality = 0):
    output_path = os.path.abspath(output_path)
    
    ydl_opts = {
        'format': 'bestvideo+bestaudio/best',
        'outtmpl': os.path.join(output_path, '%(title)s.%(ext)s'),
        'progress_hooks': [progress_hook],
        'verbose': False,
        'quiet': True, 
        'no_warnings': True
    }

    try:
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            info = ydl.extract_info(url, download=False)
            
            if 'formats' not in info:
                print("No available formats found. The video might be restricted or unavailable.")
                return

            video_formats = []
            audio_formats = []

            for f in info['formats']:
                format_info = get_format_info(f)
                if format_info:
                    if format_info['vcodec'] != 'none' and format_info['acodec'] == 'none':
                        video_formats.append((format_info['height'], format_info))
                    elif format_info['vcodec'] == 'none' and format_info['acodec'] != 'none':
                        audio_formats.append(format_info)

            video_formats.sort(key=lambda x: x[0], reverse=True)
            
            if not video_formats:
                print("No suitable video formats found. The video might be restricted or unavailable.")
                return



            if quality == 2:
                video_choice = 1
            else:
                print("\nAvailable video qualities:")
                for i, (height, f) in enumerate(video_formats):
                    print(f"{i + 1}. {height}p (Format ID: {f['format_id']}, Codec: {f['vcodec']})")
                video_choice = int(input("\nEnter the number of the video quality you want to download: ")) - 1

            if video_choice < 0 or video_choice >= len(video_formats):
                print("Invalid choice. Please run the script again and select a valid option.")
                return

            chosen_video_format = video_formats[video_choice][1]['format_id']
            
            if not audio_formats:
                print("No suitable audio formats found. The video might be restricted or unavailable.")
                return
            audio_format = audio_formats[0]['format_id']

            safe_title = sanitize_filename(info['title'])
            video_filename = os.path.join(output_path, f"{safe_title}_video.{video_formats[video_choice][1]['ext']}")
            audio_filename = os.path.join(output_path, f"{safe_title}_audio.m4a")
            output_filename = os.path.join(output_path, f"{safe_title}.mp4")

            os.makedirs(output_path, exist_ok=True)
            # clear_terminal()
            print(f"\nDownloading video: {safe_title} ({video_formats[video_choice][0]}p)")
            ydl_opts['format'] = chosen_video_format
            ydl_opts['outtmpl'] = video_filename
            with yt_dlp.YoutubeDL(ydl_opts) as ydl_video:
                ydl_video.download([url])

            print("\nDownloading audio")
            ydl_opts['format'] = audio_format
            ydl_opts['outtmpl'] = audio_filename
            with yt_dlp.YoutubeDL(ydl_opts) as ydl_audio:
                ydl_audio.download([url])
            # clear_terminal()
            print("\nMerging video and audio")
            ffmpeg_cmd = ['ffmpeg', '-i', video_filename, '-i', audio_filename, '-c', 'copy', output_filename]
            subprocess.run(ffmpeg_cmd, check=True, stderr=subprocess.PIPE)

            print("\nCleaning up temporary files")
            os.remove(video_filename)
            os.remove(audio_filename)
            # clear_terminal()
        print("\nDownload and merge complete!")

    except subprocess.CalledProcessError as e:
        print(f"Error during FFmpeg processing: {e.stderr.decode()}")
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Please try again or check if the video is available.")



def download_audio(url, output_path="downloads/single_audios"):
    output_path = os.path.abspath(output_path)

    ydl_opts = {
        'format': 'bestaudio/best',
        'outtmpl': os.path.join(output_path, '%(title)s.%(ext)s'),
        'progress_hooks': [progress_hook],
        'verbose': False,
        'quiet': True,
        'no_warnings': True,
        'postprocessors': [{
            'key': 'FFmpegExtractAudio',
            'preferredcodec': 'm4a',
        }],
        'postprocessor_args': ['-b:a', '320k'],
    }

    try:
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            info = ydl.extract_info(url, download=True)

            print("\nAudio download process completed.")

            # List files in the directory
            m4a_files = [f for f in os.listdir(output_path) if f.endswith('.m4a')]

            if not m4a_files:
                print("No .m4a files found in the output directory.")
                return

            # Use the first .m4a file found
            audio_filename = os.path.join(output_path, m4a_files[0])

            # Generate output filename
            output_filename = os.path.splitext(audio_filename)[0] + '_.mp3'

            # Escape filenames for shell
            audio_filename_escaped = shlex.quote(audio_filename)
            output_filename_escaped = shlex.quote(output_filename)

            ffmpeg_cmd = f'ffmpeg -i {audio_filename_escaped} -c:a libmp3lame -b:a 320k {output_filename_escaped}'

            try:
                subprocess.run(ffmpeg_cmd, shell=True, check=True, stderr=subprocess.PIPE)
                print("Conversion successful!")
                
                os.remove(audio_filename)
            except subprocess.CalledProcessError as e:
                print(f"An error occurred during conversion: {e.stderr.decode()}")

    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Please try again or check if the video is available.")




def progress_hook(d):
    if d['status'] == 'downloading':
        percent = d['_percent_str']
        speed = d['_speed_str']
        eta = d['_eta_str']
        print(f"\rDownloading... {percent} (Speed: {speed}, ETA: {eta})", end='', flush=True)
    elif d['status'] == 'finished':
        print("\nDownload finished. Processing...")


# Function to extract video links from a playlist
def get_video_links_from_playlist(playlist_url):
    # Options to retrieve playlist information
    ydl_opts = {
        'extract_flat': True,  # Extract video URLs without downloading
        'skip_download': True,  # Skip downloading videos
        'quiet': True, 
        'no_warnings': True
    }
    
    with yt_dlp.YoutubeDL(ydl_opts) as ydl:
        playlist_info = ydl.extract_info(playlist_url, download=False)
        
        # Extract video URLs from playlist entries
        video_urls = []
        if 'entries' in playlist_info:
            for entry in playlist_info['entries']:
                video_urls.append(entry['url'])
    
    return video_urls


def get_playlist_info(playlist_url):
    # Options to retrieve playlist information
    ydl_opts = {
        'extract_flat': True,   # Extract video URLs without downloading
        'skip_download': True,  # Skip downloading videos
        'quiet': True,          # Suppress terminal output
        'no_warnings': True     # Suppress warnings
    }
    
    with yt_dlp.YoutubeDL(ydl_opts) as ydl:
        playlist_info = ydl.extract_info(playlist_url, download=False)
        
        # Extract playlist name (title)
        playlist_name = playlist_info.get('title', 'Unknown_Playlist')
        
        # Extract video URLs from playlist entries
        video_urls = []
        if 'entries' in playlist_info:
            for entry in playlist_info['entries']:
                video_urls.append(entry['url'])
    
    return playlist_name, video_urls


def clear_terminal():
    # For Windows
    if os.name == 'nt':
        os.system('cls')
    # For macOS and Linux (posix systems)
    else:
        os.system('clear')


if __name__ == "__main__": 
    choice = int(input("1.single video download\n2.whole playlist video download\n3.single audio download\n4.whole playlist audio download\nyourchoice :: "))
    clear_terminal()
    if choice == 1:
        video_url = input("Enter the YouTube video URL: ")
        download_video(video_url)
        clear_terminal()
        print("video downloaded!!")
    elif choice == 2:
        quality = int(input("quality of videos\n1.manual\n2.best\nchoice :: "))
        playlist_url = input("Enter the YouTube playlist URL: ")

        playlist_name, urls = get_playlist_info(playlist_url)
        output_path = "downloads/" + playlist_name
        clear_terminal()
        counter = 1
        
        for i in urls:
            print(f'{playlist_name} ({counter} / {len(urls)})')
            if quality == 2:
                download_video(i,output_path=output_path, quality=2)
            else:
                download_video(i)
            clear_terminal()
            counter += 1
    elif choice == 3:
        video_url = input("Enter the YouTube video URL: ")
        download_audio(video_url)
    elif choice == 4:
        playlist_url = input("Enter the YouTube playlist URL: ")

        playlist_name, urls = get_playlist_info(playlist_url)
        output_path = "downloads/" + playlist_name
        clear_terminal()
        counter = 1
        
        for i in urls:
            print(f'{playlist_name} ({counter} / {len(urls)})')
            download_audio(i, output_path=output_path)
            clear_terminal()
            counter += 1
