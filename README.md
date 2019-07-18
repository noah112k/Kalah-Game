# Kalah-Game
Full Implementation of the game Kalah, including option for an AI computer player

Object Oriented Programming approach to designing a Kalah game, with a board object, a game object, and a player object which interact with one another to keep track of score, number of beans in each hole, etc. 

The player class has three inherited subclasses, SmartPlayer, BadPlayer, and HumanPlayer. Human Player allows for player control of ones own moves, so playing the game with two HumanPlayer objects would constitute two people playing against one another in real life. BadPlayer is just a simplified/ineffective version of SmartPlayer, both of which operate without any input from the user, but the SmartPlayer uses recursion and a mini-max algorithm to look ahead at many possible outcomes and select the most optimal outcome.





