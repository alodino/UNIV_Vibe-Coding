import requests
from bs4 import BeautifulSoup
import time
import re
from urllib.parse import urljoin, quote

def get_actor_movies(actor_name):
    """
    배우 이름을 입력받아 해당 배우가 출연한 영화 리스트를 반환하는 함수
    
    Args:
        actor_name (str): 배우 이름
    
    Returns:
        list: [(영화제목, 영화URL), ...] 형태의 리스트
    """
    try:
        # IMDB 검색 URL 생성
        search_url = f"https://www.imdb.com/find/?q={quote(actor_name)}&s=nm"
        
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
        }
        
        # 검색 페이지에서 배우 찾기
        response = requests.get(search_url, headers=headers)
        response.raise_for_status()
        
        soup = BeautifulSoup(response.content, 'html.parser')
        
        # 첫 번째 배우 결과 찾기
        actor_link = None
        results = soup.find_all('td', class_='result_text')
        
        for result in results:
            link = result.find('a')
            if link and '/name/' in link.get('href', ''):
                actor_link = link.get('href')
                break
        
        if not actor_link:
            print(f"배우 '{actor_name}'을 찾을 수 없습니다.")
            return []
        
        # 배우 페이지 URL 생성
        actor_url = urljoin("https://www.imdb.com", actor_link)
        
        # 배우 페이지에서 영화 목록 가져오기
        response = requests.get(actor_url, headers=headers)
        response.raise_for_status()
        
        soup = BeautifulSoup(response.content, 'html.parser')
        
        movies = []
        
        # 영화 링크들 찾기 (다양한 섹션에서)
        movie_links = soup.find_all('a', href=re.compile(r'/title/tt\d+/'))
        
        seen_movies = set()
        
        for link in movie_links:
            movie_url = link.get('href')
            movie_title = link.get_text(strip=True)
            
            # 중복 제거 및 유효한 영화만 선택
            if (movie_url and movie_title and 
                movie_url not in seen_movies and 
                len(movie_title) > 1 and
                not movie_title.isdigit()):
                
                full_movie_url = urljoin("https://www.imdb.com", movie_url)
                movies.append((movie_title, full_movie_url))
                seen_movies.add(movie_url)
        
        print(f"'{actor_name}' 배우의 영화 {len(movies)}개를 찾았습니다.")
        return movies[:20]  # 상위 20개 영화만 반환 (성능상 이유)
        
    except Exception as e:
        print(f"배우 '{actor_name}' 검색 중 오류 발생: {e}")
        return []


def get_movie_actors(movie_url):
    """
    영화 URL을 입력받아 해당 영화에 출연한 배우 리스트를 반환하는 함수
    
    Args:
        movie_url (str): IMDB 영화 페이지 URL
    
    Returns:
        list: 배우 이름 리스트
    """
    try:
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
        }
        
        response = requests.get(movie_url, headers=headers)
        response.raise_for_status()
        
        soup = BeautifulSoup(response.content, 'html.parser')
        
        actors = []
        
        # 다양한 방법으로 출연진 정보 찾기
        
        # 방법 1: 캐스트 섹션에서 찾기
        cast_section = soup.find('section', {'data-testid': 'title-cast'})
        if cast_section:
            actor_links = cast_section.find_all('a', href=re.compile(r'/name/nm\d+/'))
            for link in actor_links:
                actor_name = link.get_text(strip=True)
                if actor_name and len(actor_name) > 1:
                    actors.append(actor_name)
        
        # 방법 2: 일반적인 배우 링크들 찾기
        if not actors:
            actor_links = soup.find_all('a', href=re.compile(r'/name/nm\d+/'))
            for link in actor_links:
                actor_name = link.get_text(strip=True)
                if (actor_name and len(actor_name) > 1 and 
                    not actor_name.isdigit() and
                    not any(char in actor_name.lower() for char in ['director', 'writer', 'producer'])):
                    actors.append(actor_name)
        
        # 중복 제거
        actors = list(dict.fromkeys(actors))  # 순서 유지하면서 중복 제거
        
        print(f"영화에서 {len(actors)}명의 배우를 찾았습니다.")
        return actors[:30]  # 상위 30명만 반환 (성능상 이유)
        
    except Exception as e:
        print(f"영화 '{movie_url}' 출연진 검색 중 오류 발생: {e}")
        return []


# 테스트 함수
def test_functions():
    """함수들을 테스트하는 함수"""
    print("=== 함수 테스트 시작 ===")
    
    # 배우의 영화 목록 테스트
    print("\n1. Tom Cruise의 영화 목록 가져오기:")
    tom_movies = get_actor_movies("Tom Cruise")
    for i, (title, url) in enumerate(tom_movies[:5]):  # 처음 5개만 출력
        print(f"  {i+1}. {title}")
    
    if tom_movies:
        print(f"\n2. 첫 번째 영화 '{tom_movies[0][0]}'의 출연진 가져오기:")
        movie_actors = get_movie_actors(tom_movies[0][1])
        for i, actor in enumerate(movie_actors[:10]):  # 처음 10명만 출력
            print(f"  {i+1}. {actor}")
    
    print("\n=== 테스트 완료 ===")

# 테스트 실행
if __name__ == "__main__":
    test_functions()
