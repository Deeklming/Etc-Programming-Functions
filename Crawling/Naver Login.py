# selenium 4+
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from webdriver_manager.chrome import ChromeDriverManager# 크롬 드라이버 자동 업데이트
import time
import pyautogui
import pyperclip

chrome_options = Options()# 옵션 선택
chrome_options.add_experimental_option("detach", True)# 브라우저 꺼짐 방지
chrome_options.add_experimental_option("excludeSwitches", ["enable-logging"])# 불필요한 에러 메시지 삭제

service = Service(executable_path=ChromeDriverManager().install())# 크롬 드라이버 설치
driver = webdriver.Chrome(service=service, options=chrome_options)# 드라이버 설정

#driver.get("https://www.naver.com")
driver.implicitly_wait(5)# 웹페이지가 로딩 될때까지 5초 기다림
driver.maximize_window()# 화면 최대화
driver.get("https://nid.naver.com/nidlogin.login?mode=form&url=https%3A%2F%2Fwww.naver.com")# 웹페이지 주소로 이동

time.sleep(1)
id = driver.find_element(By.CSS_SELECTOR, "#id")# 아이디 입력창
id.click()
#id.send_keys("") 자동 로그인 방지에 걸림
pyperclip.copy("실제 아이디")# 클립으로 복사
pyautogui.hotkey("ctrl", "v")# 붙여넣기
time.sleep(3)

pw = driver.find_element(By.CSS_SELECTOR, "#pw")# 비밀번호 입력창
pw.click()
#pw.send_keys("") 자동 로그인 방지에 걸림
pyperclip.copy("실제 비밀번호")# 클립으로 복사
pyautogui.hotkey("ctrl", "v")# 붙여넣기
time.sleep(2)

login_btn = driver.find_element(By.CSS_SELECTOR, "#log\.login")# 로그인 버튼
login_btn.click()
