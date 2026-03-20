import undetected_chromedriver as uc # Use uc instead of webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
import time

if __name__ == '__main__':
    # Initialize uc.Chrome instead of webdriver.Chrome
    # uc handles the anti-bot patches automatically
    driver = uc.Chrome()

    try:
        driver.get("https://www.google.com")
        
        # A small wait is still good practice for network loading
        time.sleep(3) 

        # Find the search box
        # Note: 'q' is a more stable name attribute for Google's search box
        search_box = driver.find_element(By.NAME, "q")
        
        # Type the text and press Enter
        search_box.send_keys('CampusX' + Keys.ENTER)

        # go to campusx link 
        link = driver.find_element(by=By.XPATH, value='//*[@id="rso"]/div[1]/div/div/div/div/div/div/div/div[1]/div/span/a')
        link.click()

        link_inside = driver.find_element(by=By.XPATH,value='//*[@id="1698390585510d"]/div/div[1]/div/div/div/div[1]/div/div/div[2]/a[2]')
        link_inside.click()
        
        # Keep the window open for a few seconds to see results
        time.sleep(10)

    finally:
        driver.quit()