import undetected_chromedriver as uc
import time
import random
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

options = uc.ChromeOptions()
options.add_argument("--start-maximized")
driver = uc.Chrome(options=options)

try:
    driver.get("https://www.smartprix.com/mobiles")
    time.sleep(random.uniform(4, 6))

    # # Apply Filters
    # try:
    #     # Checkbox 1
    #     driver.find_element(By.XPATH, '//*[@id="app"]/main/aside/div/div[5]/div[2]/label[1]/input').click()
    #     time.sleep(1)
    #     # Checkbox 2
    #     driver.find_element(By.XPATH, '//*[@id="app"]/main/aside/div/div[5]/div[2]/label[2]/input').click()
    #     time.sleep(1)
    #     # Checkbox 4
    #     driver.find_element(By.XPATH, '//*[@id="app"]/main/aside/div/div[5]/div[2]/label[4]/input').click()
    #     time.sleep(2)
    # except Exception as e:
    #     print("Filter click failed, continuing...")

    # Initialize heights and retry counter
    old_height = 0
    consecutive_same_height_count = 0
    max_retries = 5 

    while True:
        # 1. Scroll to the bottom
        driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
        time.sleep(2)

        # 2. Get the current height to check for progress
        current_height = driver.execute_script("return document.body.scrollHeight")
        print(f"Current Height: {current_height} | Retries: {consecutive_same_height_count}")

        # Check if height has changed
        if current_height == old_height:
            consecutive_same_height_count += 1
        else:
            consecutive_same_height_count = 0 # Reset counter if we made progress
        
        # If we've hit the retry limit, exit the loop
        if consecutive_same_height_count >= max_retries:
            print("Height hasn't increased for several attempts. Ending loop.")
            break

        old_height = current_height

        try:
            # 3. Attempt to find and click 'Load More' (or 'Try Again')
            load_more = WebDriverWait(driver, 5).until(
                EC.element_to_be_clickable((By.CLASS_NAME, "sm-load-more"))
            )
            
            print("Clicking 'Load More' / 'Try Again' button...")
            driver.execute_script("arguments[0].click();", load_more)
            
            # Short wait for the server to react to the click
            time.sleep(random.uniform(1,2)) 

        except:
            # If the button is not present at all, we might be at the end
            print("Load More button not visible. Checking height one last time...")
            # We don't 'break' here immediately; we let the height check at the top of the loop decide

    # Final Save
    html = driver.page_source
    with open('smartprix.html', 'w', encoding='utf-8') as f:
        f.write(html)
    print("HTML saved successfully!")

except Exception as e:
    print(f"An error occurred: {e}")

finally:
    driver.quit()