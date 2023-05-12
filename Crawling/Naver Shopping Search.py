# selenium
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
import time

browser = webdriver.Chrome(".\\chromedriver.exe")# 브라우저 생성, 따로 다운
browser.get('https://www.naver.com/')# 웹사이트 열기
browser.implicitly_wait(5)# 로딩 끝날 때까지 5초는 기다림

browser.find_element_by_css_selector('a.nav.shop').click()# 쇼핑 메뉴 클릭
time.sleep(2)# 쇼핑창 뜰 때까지 2초 기다림

search = browser.find_element_by_css_selector('input._searchInput_search_input_QXUFf')# 검색창 클래스 선택
search.click()# 검색창 클릭

search.send_keys('갤럭시 S22')# 검색어 선택
search.send_keys(Keys.ENTER)# 엔터

#동적 사이트 스크롤
beforeHeight = browser.execute_script("return window.scrollY")# 스크롤 전 높이
while True:# 무한 동적 스크롤
    browser.find_element_by_css_selector("body").send_keys(Keys.END)#  맨 아래로 스크롤 내림
    time.sleep(2)# 스크롤 사이 로딩 시간
    afterHeight = browser.execute_script("return window.scrollY")# 스크롤 후 높이
    if afterHeight == beforeHeight:# 서로의 높이가 같다면 스크롤 끝냄
        break
    beforeHeight = afterHeight

items = browser.find_elements_by_css_selector(".basicList_info_area__17Xyo")# 상품 정보 div

for it in items:
    name = it.find_element_by_css_selector(".basicList_title__3P9Q7").text
    try:
        price = it.find_element_by_css_selector(".price_num__2WUXn").text
    except:
        price = "판매 중단"
    link = it.find_element_by_css_selector(".basicList_title__3P9Q7 > a").get_attribute('href')
    print("\n<naver shopping>", name, price)
    print("<url>", link)
