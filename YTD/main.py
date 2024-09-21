import os
import yt_dlp
import subprocess
import re

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

def download_video(url, output_path="downloads"):
    output_path = os.path.abspath(output_path)
    
    ydl_opts = {
        'format': 'bestvideo+bestaudio/best',
        'outtmpl': os.path.join(output_path, '%(title)s.%(ext)s'),
        'progress_hooks': [progress_hook],
        'verbose': True
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

            print("\nMerging video and audio")
            ffmpeg_cmd = ['ffmpeg', '-i', video_filename, '-i', audio_filename, '-c', 'copy', output_filename]
            subprocess.run(ffmpeg_cmd, check=True, stderr=subprocess.PIPE)

            print("\nCleaning up temporary files")
            os.remove(video_filename)
            os.remove(audio_filename)

        print("\nDownload and merge complete!")

    except subprocess.CalledProcessError as e:
        print(f"Error during FFmpeg processing: {e.stderr.decode()}")
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

if __name__ == "__main__": 
    video_url = input("Enter the YouTube video URL: ") 
    download_video(video_url)
