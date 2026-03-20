import undetected_chromedriver as uc
import time
import random
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys


# 1. Advanced Options to hide Selenium signatures
options = uc.ChromeOptions()
options.add_argument("--start-maximized")
options.add_argument("--disable-popup-blocking")

# 2. Initialize driver
driver = uc.Chrome(options=options)

try:
    # 3. Use a 'Human' wait before the request
    driver.get("https://www.smartprix.com/") # Visit the home page first to get cookies
    time.sleep(random.uniform(2, 4))
    
    # Now go to the specific category
    driver.get("https://www.smartprix.com/mobiles")
    time.sleep(random.uniform(2, 4))

    driver.find_element(by=By.XPATH, value='//*[@id="app"]/main/aside/div/div[5]/div[2]/label[1]/input').click()
    driver.find_element(by=By.XPATH, value='//*[@id="app"]/main/aside/div/div[5]/div[2]/label[2]/input').click()
    driver.find_element(by=By.XPATH, value='//*[@id="app"]/main/aside/div/div[5]/div[2]/label[4]/input').click()
    time.sleep(random.uniform(4, 6))

    last_height = driver.execute_script("return document.body.scrollHeight")

    while True:
        driver.find_element(by=By.XPATH, value='//*[@id="app"]/main/div[1]/div[2]/div[3]')
        time.sleep(random.uniform(1, 2))

        new_height = driver.execute_script("return document.body.scrollHeight")
        
        if new_height == last_height:
            break
        
        last_height = new_height
        
        print(f"Current Page Height: {new_height}")

except Exception as e:
    print(f"An error occurred: {e}")

finally:
    # Keep open for a bit so you can see if it worked
    time.sleep(5)
    html= driver.page_source
    with open('smartphone.html','w',encoding='utf-8') as f:
        f.write(html)
    driver.quit()