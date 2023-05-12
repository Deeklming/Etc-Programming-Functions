# simple
import requests
from bs4 import BeautifulSoup as bs

while True:
    a = input("news search? ")
    b = input("How many pages? ")
    page_num = 1
    for x in range(1, int(b)*10, 10):
        print(f"======================<page: {page_num}>=====================")
        nv = f"https://search.naver.com/search.naver?where=news&sm=tab_pge&query={a}&sort=0&photo=0&field=0&pd=0&ds=&de=&mynews=0&office_type=0&office_section_code=0&news_office_checked=&nso=so:r,p:all,a:all&start={x}"
        nvres = requests.get(nv) #url으로 요청해서 받음
        nvhtml = nvres.text #html형식으로
        nvsoup = bs(nvhtml, 'html.parser') #BeautifulSoup로 파싱
        nvlinks = nvsoup.select(".news_tit") #클래스 선택, 리스트 반환
        for n in nvlinks:
            title = n.text #타이틀
            url = n.attrs['href'] #주소
            print('<news>:', title, '  <URL>:',url)
        page_num += 1
