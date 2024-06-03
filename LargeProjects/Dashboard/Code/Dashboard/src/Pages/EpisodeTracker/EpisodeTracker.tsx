import React, { useEffect, useState } from "react";
import showsData from '../../Data/shows.json';

interface Show {
  enabled: boolean;
  show_name: string;
  season_number: number;
  episode_number: number;
}

const MainPage: React.FC = () => {
  const [searchResult, setSearchResult] = useState<Record<string, string | null>>({});
  const [shows, setShows] = useState<Show[]>([]);

  useEffect(() => {
    console.clear();

    setShows(showsData);
    // console.log(shows)

    const initialSearchResult = Object.fromEntries(
      shows
        .filter(show => show.enabled === true)
        .map(show => [show.show_name, null])
    );
    setSearchResult(initialSearchResult);

    // console.log(searchResult)

    async function searchForShow(showName, seasonNumber, episodeNumber) {
      let url = `https://nyaa.si/?page=rss&q=%5BEMBER%5D+${showName}+s${seasonNumber.toString().padStart(2, '0')}e${episodeNumber.toString().padStart(2, '0')}&c=0_0&f=0`.replace(/ /g, '+');

      try {
        let myObject = await fetch(url);
        let myText = await myObject.text();
        
        let parser = new DOMParser();
        let xml = parser.parseFromString(myText, "text/xml");
        console.log(xml)

        const items = xml.querySelectorAll('item');
        if (items.length === 0) {
          setSearchResult(prevResult => ({
            ...prevResult,
            [showName]: "No Episode Found",
          }));
        } else {
          itemsLoop: for (let i = 0; i < items.length; i++) {
            const item = items[i];
            const title = item.querySelector('title')?.textContent || '';

            console.log('Title:', title);

            // Your condition to break out of the loop
            if (title.toLowerCase().includes('ember')) {
              setSearchResult(prevResult => ({
                ...prevResult,
                [showName]: "Found episode",
              }));
              break itemsLoop; // This will break out of the labeled loop
            }
          }
        }
      } catch (error) {
        console.error("Error:", error);
        // setSearchResult("Error Occurred");
      }
    }

    shows.forEach((show) => {
      if (show.enabled === true) {
        console.log(show.show_name);

        searchForShow(show.show_name, show.season_number, show.episode_number);

      }
    });



    // Call the asynchronous fetchData function
    // fetchData();

    // Cleanup function to handle componentWillUnmount
    return () => {
      // Cleanup resources here if necessary
    };
  }, []);

  return (
    <div className='timer-page'>
      <h1>Episode Tracker</h1>
      <ul>
        {Object.entries(searchResult).map(([showName, result]) => (
          <li key={showName}>
            {showName +": "} 
            {result !== null ? (
              <>{result}</>
            ) : (
              "Data loading or parsing..."
            )}
          </li>
        ))}
      </ul>
    </div>
  );
};

export default MainPage;
