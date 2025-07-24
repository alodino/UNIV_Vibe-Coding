# web.py
import sys
import requests
from bs4 import BeautifulSoup
import re

BASE_URL = "https://www.imdb.com"

def get_top_actor_url(actor_name):
    query = "+".join(actor_name.strip().split())
    url = f"{BASE_URL}/find?q={query}&s=nm"
    headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"
    }
    res = requests.get(url, headers=headers)
    soup = BeautifulSoup(res.text, 'html.parser')
    link = soup.select_one(".findResult .result_text a")
    if not link: return None
    return BASE_URL + link['href']

def get_top_movie_url(movie_title):
    query = "+".join(movie_title.strip().split())
    url = f"{BASE_URL}/find?q={query}&s=tt"
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"
    }
    res = requests.get(url, headers=headers)
    soup = BeautifulSoup(res.text, 'html.parser')
    link = soup.select_one(".findResult .result_text a")
    if not link: return None
    return BASE_URL + link['href']

def get_movies_from_actor_page(actor_url):
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"
    }
    res = requests.get(url, headers=headers)
    soup = BeautifulSoup(res.text, 'html.parser')
    filmography = soup.select(".filmo-row b a")
    return [tag.text.strip() for tag in filmography[:10]]  # top 10 only

def get_actors_from_movie_page(movie_url):
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"
    }
    res = requests.get(url, headers=headers)
    soup = BeautifulSoup(res.text, 'html.parser')
    cast_table = soup.select("table.cast_list tr .primary_photo+td a")
    return [a.text.strip() for a in cast_table[:10] if a.text.strip()]  # top 10

def main():
    if len(sys.argv) < 3:
        print("Usage: web.py [actor|movie] [name]")
        return

    mode, name = sys.argv[1], " ".join(sys.argv[2:])
    try:
        if mode == "actor":
            url = get_top_actor_url(name)
            if not url:
                print("ERROR: Actor not found")
                return
            results = get_movies_from_actor_page(url)
        elif mode == "movie":
            url = get_top_movie_url(name)
            if not url:
                print("ERROR: Movie not found")
                return
            results = get_actors_from_movie_page(url)
        else:
            print("ERROR: Invalid mode")
            return
    except Exception as e:
        print("ERROR: Exception occurred:", str(e))
        return

    for r in results:
        print(r)

if __name__ == "__main__":
    main()
