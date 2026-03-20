import undetected_chromedriver as uc
import time
import random

# 1. Advanced Options to hide Selenium signatures
options = uc.ChromeOptions()
options.add_argument("--start-maximized")
options.add_argument("--disable-popup-blocking")

# 2. Initialize driver
driver = uc.Chrome(options=options)

try:
    # 3. Use a 'Human' wait before the request
    driver.get("https://www.ajio.com") # Visit the home page first to get cookies
    time.sleep(random.uniform(2, 4))
    
    # Now go to the specific category
    driver.get("https://www.ajio.com/men-backpacks/c/830201001")
    time.sleep(5) 

    last_height = driver.execute_script("return document.body.scrollHeight")

    while True:
        # Scroll down in small "human-like" increments instead of one big jump
        driver.execute_script("window.scrollBy(0, 800);")
        time.sleep(random.uniform(1, 3)) # Variable wait times

        new_height = driver.execute_script("return document.body.scrollHeight")
        
        # Check if we are at the bottom
        current_scroll_pos = driver.execute_script("return window.pageYOffset + window.innerHeight;")
        
        if current_scroll_pos >= new_height:
            # Try one more scroll just to be sure
            driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
            time.sleep(3)
            if driver.execute_script("return document.body.scrollHeight") == new_height:
                break
        
        print(f"Current Page Height: {new_height}")

except Exception as e:
    print(f"An error occurred: {e}")

finally:
    # Keep open for a bit so you can see if it worked
    time.sleep(10)
    html= driver.page_source
    with open('ajio.html','w',encoding='utf-8') as f:
        f.write(html)
    driver.quit()