from selenium import webdriver
from selenium.webdriver.chrome.options import Options
import time
import os
import sys

def webscreen():
    #url='https://pku.edu.cn'
    url=sys.argv[1]
    #driver=webdriver.PhantomJS()
    
    chrome_options = Options()
    chrome_options.add_argument('--headless')
    chrome_options.add_argument('--no-sandbox')
    chrome_options.add_argument('--disable-dev-shm-usage')
    chrome_options.add_argument('--disable-gpu')
    chrome_options.add_argument('--hide-scrollbars')
    driver = webdriver.Chrome(chrome_options=chrome_options)
   
    #driver = webdriver.Chrome()
    driver.set_window_size(1366,768)
    driver.get(url)
    time.sleep(3)
    print(driver.title)
    driver.get_screenshot_as_file("/home/zrain/papercpp/libcpp/paper2-master/xx.png")
    time.sleep(3)
    driver.close()
webscreen()
