import os
import yt_dlp
import subprocess
import re
import shlex

def sanitize_filename(filename):
    return re.sub(r'[\\/*?:"<>|]', "", filename)

def progress_hook(d):
    if d['status'] == 'downloading':
        percent = d['_percent_str']
        speed = d['_speed_str']
        eta = d['_eta_str']
        print(f"\rDownloading... {percent} (Speed: {speed}, ETA: {eta})", end='', flush=True)
    elif d['status'] == 'finished':
        print("\nDownload finished. Processing...")

def download_video(url, output_path="downloads/single_videos", quality=0):
    """
    Download video using yt-dlp's built-in format selection and merging
    """
    output_path = os.path.abspath(output_path)
    os.makedirs(output_path, exist_ok=True)
    
    try:
        # First, get video info to show available qualities
        with yt_dlp.YoutubeDL({'quiet': True, 'no_warnings': True}) as ydl:
            info = ydl.extract_info(url, download=False)
            
        if not info:
            print("Could not retrieve video information.")
            return
            
        print(f"\nVideo Title: {info.get('title', 'Unknown')}")
        
        # Format selection based on quality choice
        if quality == 2:  # Best quality automatically
            format_selector = 'bestvideo[height<=1080]+bestaudio/best[height<=1080]'
        else:  # Manual quality selection
            # Get available video formats
            formats = info.get('formats', [])
            video_formats = []
            
            for f in formats:
                if (f.get('vcodec') != 'none' and 
                    f.get('height') and 
                    f.get('height') > 0):
                    video_formats.append({
                        'height': f.get('height'),
                        'format_id': f.get('format_id'),
                        'vcodec': f.get('vcodec', 'unknown'),
                        'fps': f.get('fps', 'unknown')
                    })
            
            # Remove duplicates and sort by height
            seen_heights = set()
            unique_formats = []
            for fmt in sorted(video_formats, key=lambda x: x['height'], reverse=True):
                if fmt['height'] not in seen_heights:
                    unique_formats.append(fmt)
                    seen_heights.add(fmt['height'])
            
            if not unique_formats:
                print("No suitable video formats found.")
                return
                
            print("\nAvailable video qualities:")
            for i, fmt in enumerate(unique_formats):
                print(f"{i + 1}. {fmt['height']}p (Codec: {fmt['vcodec']}, FPS: {fmt['fps']})")
            
            while True:
                try:
                    choice = int(input("\nEnter the number of the video quality you want to download: ")) - 1
                    if 0 <= choice < len(unique_formats):
                        selected_height = unique_formats[choice]['height']
                        # Use height-based selection with automatic audio merging
                        format_selector = f'bestvideo[height<={selected_height}]+bestaudio/best[height<={selected_height}]'
                        break
                    else:
                        print("Invalid choice. Please try again.")
                except ValueError:
                    print("Please enter a valid number.")
        
        # Download with automatic merging
        ydl_opts = {
            'format': format_selector,
            'outtmpl': os.path.join(output_path, '%(title)s.%(ext)s'),
            'progress_hooks': [progress_hook],
            'merge_output_format': 'mp4',  # Ensure output is mp4
            'writesubtitles': False,
            'writeautomaticsub': False,
            'verbose': False,
            'quiet': True,
            'no_warnings': True
        }
        
        print(f"\nDownloading and merging video...")
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            ydl.download([url])
            
        print("\nDownload and merge complete!")
        
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Please try again or check if the video is available.")

def download_audio(url, output_path="downloads/single_audios"):
    """
    Download audio using yt-dlp's built-in audio extraction
    """
    output_path = os.path.abspath(output_path)
    os.makedirs(output_path, exist_ok=True)
    
    ydl_opts = {
        'format': 'bestaudio/best',
        'outtmpl': os.path.join(output_path, '%(title)s.%(ext)s'),
        'progress_hooks': [progress_hook],
        'verbose': False,
        'quiet': True,
        'no_warnings': True,
        'postprocessors': [{
            'key': 'FFmpegExtractAudio',
            'preferredcodec': 'mp3',
            'preferredquality': '320',
        }],
    }
    
    try:
        print(f"\nDownloading and converting audio...")
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            ydl.download([url])
        print("\nAudio download and conversion complete!")
        
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Please try again or check if the video is available.")

def get_playlist_info(playlist_url):
    """
    Extract playlist information and video URLs
    """
    ydl_opts = {
        'extract_flat': True,
        'skip_download': True,
        'quiet': True,
        'no_warnings': True
    }
    
    try:
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            playlist_info = ydl.extract_info(playlist_url, download=False)
        
        playlist_name = playlist_info.get('title', 'Unknown_Playlist')
        # Sanitize playlist name for folder creation
        playlist_name = sanitize_filename(playlist_name)
        
        video_urls = []
        if 'entries' in playlist_info:
            for entry in playlist_info['entries']:
                if entry:  # Skip None entries
                    video_urls.append(entry['url'])
        
        return playlist_name, video_urls
    
    except Exception as e:
        print(f"Error getting playlist info: {str(e)}")
        return None, []

def download_playlist_videos(playlist_url, quality=0):
    """
    Download all videos from a playlist
    """
    playlist_name, urls = get_playlist_info(playlist_url)
    
    if not urls:
        print("No videos found in playlist or error occurred.")
        return
        
    output_path = os.path.join("downloads", playlist_name)
    print(f"\nPlaylist: {playlist_name}")
    print(f"Found {len(urls)} videos")
    print(f"Output path: {output_path}")
    
    successful_downloads = 0
    failed_downloads = 0
    
    for i, video_url in enumerate(urls, 1):
        print(f'\n{"="*50}')
        print(f'Downloading video {i}/{len(urls)} from playlist: {playlist_name}')
        print(f'{"="*50}')
        
        try:
            download_video(video_url, output_path=output_path, quality=quality)
            successful_downloads += 1
        except Exception as e:
            print(f"Failed to download video {i}: {str(e)}")
            failed_downloads += 1
            continue
    
    print(f'\n{"="*50}')
    print(f'Playlist download complete!')
    print(f'Successful: {successful_downloads}/{len(urls)}')
    print(f'Failed: {failed_downloads}/{len(urls)}')
    print(f'{"="*50}')

def download_playlist_audios(playlist_url):
    """
    Download all audios from a playlist
    """
    playlist_name, urls = get_playlist_info(playlist_url)
    
    if not urls:
        print("No videos found in playlist or error occurred.")
        return
        
    output_path = os.path.join("downloads", playlist_name + "_audios")
    print(f"\nPlaylist: {playlist_name}")
    print(f"Found {len(urls)} videos")
    print(f"Output path: {output_path}")
    
    successful_downloads = 0
    failed_downloads = 0
    
    for i, video_url in enumerate(urls, 1):
        print(f'\n{"="*50}')
        print(f'Downloading audio {i}/{len(urls)} from playlist: {playlist_name}')
        print(f'{"="*50}')
        
        try:
            download_audio(video_url, output_path=output_path)
            successful_downloads += 1
        except Exception as e:
            print(f"Failed to download audio {i}: {str(e)}")
            failed_downloads += 1
            continue
    
    print(f'\n{"="*50}')
    print(f'Playlist audio download complete!')
    print(f'Successful: {successful_downloads}/{len(urls)}')
    print(f'Failed: {failed_downloads}/{len(urls)}')
    print(f'{"="*50}')

def clear_terminal():
    """Clear terminal screen"""
    if os.name == 'nt':
        os.system('cls')
    else:
        os.system('clear')

def main():
    """Main function with improved user interface"""
    print("YouTube Downloader (yt-dlp)")
    print("=" * 30)
    
    while True:
        try:
            print("\nOptions:")
            print("1. Single video download")
            print("2. Whole playlist video download")
            print("3. Single audio download")
            print("4. Whole playlist audio download")
            print("5. Exit")
            
            choice = input("\nYour choice (1-5): ").strip()
            
            if choice == '1':
                video_url = input("Enter the YouTube video URL: ").strip()
                if video_url:
                    download_video(video_url)
                    print("Video downloaded!")
                else:
                    print("Invalid URL.")
                    
            elif choice == '2':
                quality_choice = input("\nQuality options:\n1. Manual selection for each video\n2. Best available quality\nChoice (1-2): ").strip()
                
                if quality_choice in ['1', '2']:
                    playlist_url = input("Enter the YouTube playlist URL: ").strip()
                    if playlist_url:
                        quality = 2 if quality_choice == '2' else 0
                        download_playlist_videos(playlist_url, quality)
                    else:
                        print("Invalid URL.")
                else:
                    print("Invalid choice.")
                    
            elif choice == '3':
                video_url = input("Enter the YouTube video URL: ").strip()
                if video_url:
                    download_audio(video_url)
                    print("Audio downloaded!")
                else:
                    print("Invalid URL.")
                    
            elif choice == '4':
                playlist_url = input("Enter the YouTube playlist URL: ").strip()
                if playlist_url:
                    download_playlist_audios(playlist_url)
                else:
                    print("Invalid URL.")
                    
            elif choice == '5':
                print("Goodbye!")
                break
                
            else:
                print("Invalid choice. Please select 1-5.")
                
        except KeyboardInterrupt:
            print("\n\nOperation cancelled by user.")
            break
        except Exception as e:
            print(f"An unexpected error occurred: {str(e)}")

if __name__ == "__main__":
    main()

