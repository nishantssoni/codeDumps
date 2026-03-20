from selenium import webdriver

from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys


# Selenium Manager automatically finds/downloads the driver for you
driver = webdriver.Edge()
import time

try:
    driver.get("https://www.google.com")
    
    # A small wait is still good practice for network loading
    time.sleep(3) 

    # Find the search box
    # Note: 'q' is a more stable name attribute for Google's search box
    search_box = driver.find_element(By.NAME, "q")
    
    # Type the text and press Enter
    search_box.send_keys('CampusX' + Keys.ENTER)
    
    print("Search submitted successfully using undetected-chromedriver!")
    
    # Keep the window open for a few seconds to see results
    time.sleep(10)
finally:
    driver.quit()
