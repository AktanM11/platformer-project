<h1 align=center> COM-119 2025 Project 1: platformer-project  story time </h1>
<h3>At the beginning, I watched pre-last extra lecture about Project #1, after which I implemented enemy and enemymanager class in singleton style, thanks to Toksaitov explanation and his code review. It was 2 weeks ago.
Then I started implementing level class on the 15th May, through Singleton (with arguments only Level current_level;  char* current_level_data; std::vector (Level) LEVELS;)
But there was a problem, after implementing  class, It was running, but there were no cells (only air) of anything (Only player appeared).
After all, it's appeared that there was logic issue (I accidentally did it, i confused x and y), so i fixed the rows and column in for loop of Load_level() function. 
Then, I tried to make a player class, but after implementing all getters and setters in functions, player's sprite was going through the floor like a Ghost.
also i had some troubles with Jumping (i accidentally wrote {x, x} not {x, y} so everything's gone in a wrong way). So i fixed it eventually.
The hardest part was the Dynamic Level loading, cuz i was working on it the whole day. It was not working with logic and just picture of player appeared with the sky on the background.
i guess i incorrectly loaded characters through for loop in string array and counters so it lied sprites on each cell (as wall, air and dark wall)
</h3> 
