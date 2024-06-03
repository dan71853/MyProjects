import React, { useEffect, useState } from "react";
import showsData from '../../Data/shows.json';

interface Show {
    enabled: boolean;
    show_name: string;
    season_number: number;
    episode_number: number;
}

const MainPage: React.FC = () => {
    const [shows, setShows] = useState<Show[]>([]);

    useEffect(() => {
        // Load the shows data from the imported JSON file
        setShows(showsData);

        

        // Cleanup to prevent memory leaks
        return () => {
            // You can add cleanup logic here if needed
        };
    }, []);

    return (
        <div className='timer-page'>
            <h1>Episode Tracker</h1>
            <ul>
                {shows.map((show, index) => (
                    <li key={index}>
                        {show.enabled ? '✅' : '❌'} {show.show_name} - Season {show.season_number}, Episode {show.episode_number}
                    </li>
                ))}
            </ul>
        </div>
    );
};

export default MainPage;
