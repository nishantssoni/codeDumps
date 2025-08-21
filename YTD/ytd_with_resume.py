import os
import yt_dlp
import subprocess
import re
import shlex
import json
import glob
from pathlib import Path

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

def file_exists_with_extensions(base_path, title):
    """Check if file exists with common video/audio extensions"""
    extensions = ['.mp4', '.webm', '.mkv', '.avi', '.mov', '.mp3', '.m4a', '.wav', '.flac']
    sanitized_title = sanitize_filename(title)
    
    for ext in extensions:
        file_path = os.path.join(base_path, f"{sanitized_title}{ext}")
        if os.path.exists(file_path):
            return True, file_path
    
    # Also check for files that might have similar names (partial matches)
    pattern = os.path.join(base_path, f"*{sanitized_title[:30]}*")
    matching_files = glob.glob(pattern)
    if matching_files:
        return True, matching_files[0]
    
    return False, None

def get_video_title(url):
    """Get video title without downloading"""
    try:
        with yt_dlp.YoutubeDL({'quiet': True, 'no_warnings': True}) as ydl:
            info = ydl.extract_info(url, download=False)
            return info.get('title', 'Unknown')
    except:
        return 'Unknown'

def download_video(url, output_path="downloads/single_videos", quality=0, check_existing=True):
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
            return False
            
        video_title = info.get('title', 'Unknown')
        print(f"\nVideo Title: {video_title}")
        
        # Check if file already exists
        if check_existing:
            exists, existing_path = file_exists_with_extensions(output_path, video_title)
            if exists:
                print(f"✓ File already exists: {os.path.basename(existing_path)}")
                print("Skipping download...")
                return True
        
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
                return False
                
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
                except KeyboardInterrupt:
                    print("\nOperation cancelled.")
                    return False
        
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
            'no_warnings': True,
            'continuedl': True,  # Enable resume for partial downloads
        }
        
        print(f"\nDownloading and merging video...")
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            ydl.download([url])
            
        print("\nDownload and merge complete!")
        return True
        
    except KeyboardInterrupt:
        print("\nDownload interrupted by user.")
        return False
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Please try again or check if the video is available.")
        return False

def download_audio(url, output_path="downloads/single_audios", check_existing=True):
    """
    Download audio using yt-dlp's built-in audio extraction
    """
    output_path = os.path.abspath(output_path)
    os.makedirs(output_path, exist_ok=True)
    
    try:
        # Get video info first
        with yt_dlp.YoutubeDL({'quiet': True, 'no_warnings': True}) as ydl:
            info = ydl.extract_info(url, download=False)
            
        if not info:
            print("Could not retrieve video information.")
            return False
            
        video_title = info.get('title', 'Unknown')
        
        # Check if audio file already exists
        if check_existing:
            exists, existing_path = file_exists_with_extensions(output_path, video_title)
            if exists:
                print(f"✓ Audio file already exists: {os.path.basename(existing_path)}")
                print("Skipping download...")
                return True
    
        ydl_opts = {
            'format': 'bestaudio/best',
            'outtmpl': os.path.join(output_path, '%(title)s.%(ext)s'),
            'progress_hooks': [progress_hook],
            'verbose': False,
            'quiet': True,
            'no_warnings': True,
            'continuedl': True,  # Enable resume for partial downloads
            'postprocessors': [{
                'key': 'FFmpegExtractAudio',
                'preferredcodec': 'mp3',
                'preferredquality': '320',
            }],
        }
        
        print(f"\nDownloading and converting audio...")
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            ydl.download([url])
        print("\nAudio download and conversion complete!")
        return True
        
    except KeyboardInterrupt:
        print("\nDownload interrupted by user.")
        return False
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Please try again or check if the video is available.")
        return False

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
        
        videos_info = []
        if 'entries' in playlist_info:
            for entry in playlist_info['entries']:
                if entry:  # Skip None entries
                    videos_info.append({
                        'url': entry['url'],
                        'title': entry.get('title', 'Unknown')
                    })
        
        return playlist_name, videos_info
    
    except Exception as e:
        print(f"Error getting playlist info: {str(e)}")
        return None, []

def save_progress(playlist_name, completed_videos, playlist_url=None, download_type="video", quality=0):
    """Save download progress to resume later"""
    progress_file = os.path.join("downloads", f"{playlist_name}_progress.json")
    progress_data = {
        'completed_videos': completed_videos,
        'playlist_url': playlist_url,
        'download_type': download_type,
        'quality': quality,
        'last_updated': str(Path().absolute()),
        'total_completed': len(completed_videos)
    }
    
    try:
        os.makedirs("downloads", exist_ok=True)
        with open(progress_file, 'w', encoding='utf-8') as f:
            json.dump(progress_data, f, ensure_ascii=False, indent=2)
    except Exception as e:
        print(f"Warning: Could not save progress: {e}")

def load_progress(playlist_name):
    """Load previous download progress"""
    progress_file = os.path.join("downloads", f"{playlist_name}_progress.json")
    if os.path.exists(progress_file):
        try:
            with open(progress_file, 'r', encoding='utf-8') as f:
                data = json.load(f)
                # Handle old format (just list) and new format (dict)
                if isinstance(data, list):
                    return data, None, "video", 0
                else:
                    return (data.get('completed_videos', []), 
                           data.get('playlist_url'), 
                           data.get('download_type', 'video'),
                           data.get('quality', 0))
        except Exception as e:
            print(f"Warning: Could not load progress: {e}")
    return [], None, "video", 0

def download_playlist_videos(playlist_url, quality=0):
    """
    Download all videos from a playlist with resume capability
    """
    playlist_name, videos_info = get_playlist_info(playlist_url)
    
    if not videos_info:
        print("No videos found in playlist or error occurred.")
        return
        
    output_path = os.path.join("downloads", playlist_name)
    print(f"\nPlaylist: {playlist_name}")
    print(f"Found {len(videos_info)} videos")
    print(f"Output path: {output_path}")
    
    # Load previous progress
    completed_videos, saved_url, saved_type, saved_quality = load_progress(playlist_name)
    print(f"Previously completed: {len(completed_videos)} videos")
    
    successful_downloads = len(completed_videos)
    failed_downloads = 0
    
    try:
        for i, video_info in enumerate(videos_info, 1):
            video_url = video_info['url']
            video_title = video_info['title']
            
            # Skip if already completed
            if video_title in completed_videos:
                print(f"\n✓ Skipping {i}/{len(videos_info)}: {video_title} (already downloaded)")
                continue
            
            print(f'\n{"="*50}')
            print(f'Downloading video {i}/{len(videos_info)} from playlist: {playlist_name}')
            print(f'Title: {video_title}')
            print(f'{"="*50}')
            
            try:
                success = download_video(video_url, output_path=output_path, quality=quality)
                if success:
                    successful_downloads += 1
                    completed_videos.append(video_title)
                    save_progress(playlist_name, completed_videos, playlist_url, "video", quality)
                else:
                    failed_downloads += 1
            except KeyboardInterrupt:
                print(f"\nPlaylist download interrupted by user.")
                print(f"Progress saved. You can resume later.")
                save_progress(playlist_name, completed_videos, playlist_url, "video", quality)
                return
            except Exception as e:
                print(f"Failed to download video {i}: {str(e)}")
                failed_downloads += 1
                continue
        
        # Clean up progress file when complete
        progress_file = os.path.join("downloads", f"{playlist_name}_progress.json")
        if os.path.exists(progress_file):
            os.remove(progress_file)
            
    except KeyboardInterrupt:
        print(f"\nPlaylist download interrupted by user.")
        save_progress(playlist_name, completed_videos, playlist_url, "video", quality)
        return
    
    print(f'\n{"="*50}')
    print(f'Playlist download complete!')
    print(f'Successful: {successful_downloads}/{len(videos_info)}')
    print(f'Failed: {failed_downloads}/{len(videos_info)}')
    print(f'{"="*50}')

def download_playlist_audios_fast(playlist_url):
    """
    Download all audios from a playlist with optimized settings and resume capability
    """
    playlist_name, videos_info = get_playlist_info(playlist_url)
    
    if not videos_info:
        print("No videos found in playlist or error occurred.")
        return
        
    output_path = os.path.join("downloads", playlist_name + "_audios")
    print(f"\nPlaylist: {playlist_name}")
    print(f"Found {len(videos_info)} videos")
    print(f"Output path: {output_path}")
    
    # Load previous progress
    progress_key = playlist_name + "_audios"
    completed_videos, saved_url, saved_type, saved_quality = load_progress(progress_key)
    print(f"Previously completed: {len(completed_videos)} audios")
    
    successful_downloads = len(completed_videos)
    failed_downloads = 0
    
    # Optimized yt-dlp options for faster audio processing
    ydl_opts = {
        'format': 'bestaudio[ext=m4a]/bestaudio/best',  # Prefer m4a for faster conversion
        'outtmpl': os.path.join(output_path, '%(title)s.%(ext)s'),
        'progress_hooks': [progress_hook],
        'verbose': False,
        'quiet': True,
        'no_warnings': True,
        'continuedl': True,
        'extract_flat': False,
        'postprocessors': [{
            'key': 'FFmpegExtractAudio',
            'preferredcodec': 'mp3',
            'preferredquality': '192',  # Slightly lower quality for faster conversion
        }],
        'postprocessor_args': [
            '-threads', '4',  # Use multiple threads for faster processing
            '-preset', 'ultrafast',  # Fastest encoding preset
        ],
    }
    
    os.makedirs(output_path, exist_ok=True)
    
    try:
        for i, video_info in enumerate(videos_info, 1):
            video_url = video_info['url']
            video_title = video_info['title']
            
            # Skip if already completed
            if video_title in completed_videos:
                print(f"\n✓ Skipping {i}/{len(videos_info)}: {video_title} (already downloaded)")
                continue
            
            # Check if file already exists
            exists, existing_path = file_exists_with_extensions(output_path, video_title)
            if exists:
                print(f"\n✓ Skipping {i}/{len(videos_info)}: {video_title} (file exists)")
                completed_videos.append(video_title)
                successful_downloads += 1
                save_progress(progress_key, completed_videos, playlist_url, "audio", 0)
                continue
            
            print(f'\n{"="*50}')
            print(f'Downloading audio {i}/{len(videos_info)} from playlist: {playlist_name}')
            print(f'Title: {video_title}')
            print(f'{"="*50}')
            
            try:
                with yt_dlp.YoutubeDL(ydl_opts) as ydl:
                    ydl.download([video_url])
                
                successful_downloads += 1
                completed_videos.append(video_title)
                save_progress(progress_key, completed_videos, playlist_url, "audio", 0)
                print("Audio download and conversion complete!")
                
            except KeyboardInterrupt:
                print(f"\nPlaylist audio download interrupted by user.")
                print(f"Progress saved. You can resume later.")
                save_progress(progress_key, completed_videos, playlist_url, "audio", 0)
                return
            except Exception as e:
                print(f"Failed to download audio {i}: {str(e)}")
                failed_downloads += 1
                continue
        
        # Clean up progress file when complete
        progress_file = os.path.join("downloads", f"{progress_key}_progress.json")
        if os.path.exists(progress_file):
            os.remove(progress_file)
            
    except KeyboardInterrupt:
        print(f"\nPlaylist audio download interrupted by user.")
        save_progress(progress_key, completed_videos, playlist_url, "audio", 0)
        return
    
    print(f'\n{"="*50}')
    print(f'Playlist audio download complete!')
    print(f'Successful: {successful_downloads}/{len(videos_info)}')
    print(f'Failed: {failed_downloads}/{len(videos_info)}')
    print(f'{"="*50}')

def download_playlist_audios(playlist_url):
    """Wrapper for backward compatibility"""
    download_playlist_audios_fast(playlist_url)

def clear_terminal():
    """Clear terminal screen"""
    if os.name == 'nt':
        os.system('cls')
    else:
        os.system('clear')

def resume_downloads():
    """Show available downloads to resume and allow user to select"""
    downloads_dir = "downloads"
    if not os.path.exists(downloads_dir):
        print("No downloads directory found. No downloads to resume.")
        return
    
    progress_files = glob.glob(os.path.join(downloads_dir, "*_progress.json"))
    if not progress_files:
        print("No interrupted downloads found to resume.")
        return
    
    print("\n📁 Available downloads to resume:")
    print("="*50)
    
    resume_options = []
    for i, progress_file in enumerate(progress_files, 1):
        try:
            with open(progress_file, 'r', encoding='utf-8') as f:
                data = json.load(f)
            
            # Handle both old and new format
            if isinstance(data, list):
                completed_count = len(data)
                download_type = "Unknown"
                playlist_name = os.path.basename(progress_file).replace('_progress.json', '')
                playlist_url = None
                quality = 0
            else:
                completed_count = data.get('total_completed', len(data.get('completed_videos', [])))
                download_type = data.get('download_type', 'video').title()
                playlist_name = os.path.basename(progress_file).replace('_progress.json', '')
                playlist_url = data.get('playlist_url')
                quality = data.get('quality', 0)
            
            quality_text = ""
            if download_type == "Video":
                quality_text = f" (Quality: {'Best' if quality == 2 else 'Manual'})"
            
            print(f"{i}. {playlist_name}")
            print(f"   Type: {download_type}{quality_text}")
            print(f"   Completed: {completed_count} files")
            if playlist_url:
                print(f"   URL: {playlist_url[:80]}{'...' if len(playlist_url) > 80 else ''}")
            else:
                print(f"   URL: Not saved (old format)")
            print()
            
            resume_options.append({
                'file': progress_file,
                'name': playlist_name,
                'type': download_type.lower(),
                'url': playlist_url,
                'quality': quality,
                'completed': completed_count
            })
            
        except Exception as e:
            print(f"Error reading {progress_file}: {e}")
    
    if not resume_options:
        print("No valid resume options found.")
        return
    
    try:
        choice = input(f"\nSelect download to resume (1-{len(resume_options)}) or 0 to cancel: ").strip()
        
        if choice == '0':
            print("Resume cancelled.")
            return
        
        choice_idx = int(choice) - 1
        if 0 <= choice_idx < len(resume_options):
            selected = resume_options[choice_idx]
            
            if not selected['url']:
                print("⚠️  This download doesn't have a saved URL (old format).")
                url = input("Please enter the playlist URL to resume: ").strip()
                if not url:
                    print("No URL provided. Cannot resume.")
                    return
                selected['url'] = url
            
            print(f"\n🔄 Resuming: {selected['name']}")
            print(f"Type: {selected['type'].title()}")
            print(f"Completed files: {selected['completed']}")
            print(f"URL: {selected['url']}")
            
            # Resume the appropriate download type
            if selected['type'] == 'video':
                print(f"\nResuming video playlist download...")
                download_playlist_videos(selected['url'], selected['quality'])
            elif selected['type'] == 'audio':
                print(f"\nResuming audio playlist download...")
                download_playlist_audios_fast(selected['url'])
            else:
                print("Unknown download type. Cannot resume.")
                
        else:
            print("Invalid choice.")
            
    except ValueError:
        print("Please enter a valid number.")
    except KeyboardInterrupt:
        print("\nResume cancelled by user.")
    except Exception as e:
        print(f"Error during resume: {e}")

def main():
    """Main function with improved user interface and resume capability"""
    print("YouTube Downloader (yt-dlp) - Enhanced with Resume Feature")
    print("=" * 60)
    
    # Show resume status on startup
    downloads_dir = "downloads"
    if os.path.exists(downloads_dir):
        progress_files = glob.glob(os.path.join(downloads_dir, "*_progress.json"))
        if progress_files:
            print(f"\n📋 {len(progress_files)} interrupted download(s) available for resume")
    
    while True:
        try:
            print("\nOptions:")
            print("1. Single video download")
            print("2. Whole playlist video download")
            print("3. Single audio download")
            print("4. Whole playlist audio download (Fast & Resume)")
            print("5. Resume interrupted downloads")
            print("6. Exit")
            
            choice = input("\nYour choice (1-6): ").strip()
            
            if choice == '1':
                video_url = input("Enter the YouTube video URL: ").strip()
                if video_url:
                    success = download_video(video_url)
                    if success:
                        print("✓ Video downloaded successfully!")
                    else:
                        print("✗ Video download failed.")
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
                    success = download_audio(video_url)
                    if success:
                        print("✓ Audio downloaded successfully!")
                    else:
                        print("✗ Audio download failed.")
                else:
                    print("Invalid URL.")
                    
            elif choice == '4':
                playlist_url = input("Enter the YouTube playlist URL: ").strip()
                if playlist_url:
                    download_playlist_audios_fast(playlist_url)
                else:
                    print("Invalid URL.")
            
            elif choice == '5':
                resume_downloads()
                
            elif choice == '6':
                print("Goodbye!")
                break
                
            else:
                print("Invalid choice. Please select 1-6.")
                
        except KeyboardInterrupt:
            print("\n\nOperation cancelled by user.")
            break
        except Exception as e:
            print(f"An unexpected error occurred: {str(e)}")

if __name__ == "__main__":
    main()