/*///////////////////////////////////////////////////
  /$$$$$$  /$$                                                                         
 /$$__  $$| $$                                                                         
| $$  \__/| $$$$$$$   /$$$$$$   /$$$$$$$ /$$$$$$$  /$$$$$$  /$$$$$$$                   
| $$ /$$$$| $$__  $$ |____  $$ /$$_____//$$_____/ |____  $$| $$__  $$                  
| $$|_  $$| $$  \ $$  /$$$$$$$|  $$$$$$|  $$$$$$   /$$$$$$$| $$  \ $$                  
| $$  \ $$| $$  | $$ /$$__  $$ \____  $$\____  $$ /$$__  $$| $$  | $$                  
|  $$$$$$/| $$  | $$|  $$$$$$$ /$$$$$$$//$$$$$$$/|  $$$$$$$| $$  | $$                  
 \______/ |__/  |__/ \_______/|_______/|_______/  \_______/|__/  |__/                  
                                                                                       
                                                                                       
                                                                                       
 /$$      /$$                                    /$$                                   
| $$$    /$$$                                   | $$                                   
| $$$$  /$$$$ /$$   /$$  /$$$$$$  /$$$$$$   /$$$$$$$                                   
| $$ $$/$$ $$| $$  | $$ /$$__  $$|____  $$ /$$__  $$                                   
| $$  $$$| $$| $$  | $$| $$  \__/ /$$$$$$$| $$  | $$                                   
| $$\  $ | $$| $$  | $$| $$      /$$__  $$| $$  | $$                                   
| $$ \/  | $$|  $$$$$$/| $$     |  $$$$$$$|  $$$$$$$                                   
|__/     |__/ \______/ |__/      \_______/ \_______/                                   
                                                                                       
                                                                                       
                                                                                       
  /$$$$$$            /$$                                                               
 /$$__  $$          | $$                                                               
| $$  \ $$  /$$$$$$ | $$$$$$$   /$$$$$$                                                
| $$$$$$$$ /$$__  $$| $$__  $$ |____  $$                                               
| $$__  $$| $$  \ $$| $$  \ $$  /$$$$$$$                                               
| $$  | $$| $$  | $$| $$  | $$ /$$__  $$                                               
| $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$$                                               
|__/  |__/ \____  $$|__/  |__/ \_______/                                               
           /$$  \ $$                                                                   
          |  $$$$$$/                                                                   
           \______/                                                                    
                                                                                       

Free for full use...

Email: ghassanjmurad@gmail.com

Linkedin: https://www.linkedin.com/in/ghassan-murad-agha-579a3730b/

GitHub: https://github.com/Ghassan-top-dev
                                                                       
                                                                                                                                                                                                     
                                                                                                                                                                


///////////////////////////////////////////////////*/

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Screen dimension constants
const int SCREEN_WIDTH = 249;  
const int SCREEN_HEIGHT = 450; 

const int BUTTON_WIDTH = 83; //button width 83 (3 buttons horizontally)
const int BUTTON_HEIGHT = 60; //button height 66 (6 buttons vertically) //18 buttons total

const int NUM_BUTTONS = 18; //total buttons

const int BUTTON_MID_X = 35; //used to place text in the middle of the button
const int BUTTON_MID_Y = 20;

int BUTTON_TO_CHANGE_COLOR = -1; 



// Texture wrapper structure to hold texture data and dimensions
typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
} LTexture;

// Function declarations for initialization, media loading, cleanup, texture operations, and logic
bool init(); // Initializes SDL, window, and renderer
bool loadMedia(); // Loads media (e.g., font and text)
void close(); // Frees resources and shuts down SDL
bool loadFromFile(LTexture* lTexture, const char* path); // Loads image from file into texture
bool loadFromRenderedText(LTexture* lTexture, const char* textureText, SDL_Color textColor); // Renders text as texture
void freeTexture(LTexture* lTexture); // Frees texture memory
void renderTexture(LTexture* lTexture, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip); // Renders texture to screen
int getTextureWidth(LTexture* lTexture); // Returns texture width
int getTextureHeight(LTexture* lTexture); // Returns texture height
void buttonDrawer(SDL_Renderer* renderer, SDL_Rect buttons[], int count); //draws the button on the screen
void grid(); //used to draw the canvas of the calculator using lines
char whichButtonWasPressed(int buttonX, int buttonY); //used to calculate where button was pressed and on which button
void whichOperatorWasUsed(char * presented); //main meat and potatoes of the calculator logic
void colorChanger(int buttonX, int buttonY, SDL_Renderer* renderer, SDL_Rect buttons[], int R, int G, int B, int whichButton); //color change on press function
void removeLetter(char * str, char charToRemove); // may cause out of bounds problems


// Global variables for the SDL window, renderer, font, and text texture
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
LTexture inputLine; // Texture to display text
SDL_Color textColor = {0, 0, 0}; // Black text color
LTexture allOfTheButtonsTextTextures[NUM_BUTTONS];  // Array to store text textures
const char* eachText[NUM_BUTTONS] = { //the content of each button
    "X", "0", ".", "1", "2", 
    "3", "4", "5", "6", "7",
    "8", "9", "+", "-", "*", 
    "=", "CE", "/"
};



// Initializes SDL, creates window and renderer, sets up image and text libraries
bool init() {
    bool success = true; // Success flag for function

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // Set linear texture filtering
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!\n");
        }

        // Create SDL window
        gWindow = SDL_CreateWindow("SDL Calculator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            // Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); // White background

                // Initialize SDL_image with PNG support
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                // Initialize SDL_ttf for text rendering
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}



// Loads the necessary media resources such as fonts and text textures
bool loadMedia() {
    bool success = true;

    // Open the font file at size 28
    //MAKE SURE YOU ADD THE PATH TO YOUR FONT
    gFont = TTF_OpenFont("/path/to/your/font", 20); //font size
    if (gFont == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    } else {


        if (!loadFromRenderedText(&inputLine, " ", textColor)) {
            printf("Failed to render text texture!\n");
            success = false;
        }

        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (!loadFromRenderedText(&allOfTheButtonsTextTextures[i], eachText[i], textColor)) {
                printf("Failed to render text texture!\n");
                success = false;
            }
        }


    }
    return success;
}

// Frees up resources and shuts down SDL libraries
void close() {
    freeTexture(&inputLine); // Free text texture


    for (int i = 0; i < NUM_BUTTONS; i++) {
        freeTexture(&allOfTheButtonsTextTextures[i]); // Free text texture
    }

    TTF_CloseFont(gFont); // Close font
    gFont = NULL;

    SDL_DestroyRenderer(gRenderer); // Destroy renderer
    SDL_DestroyWindow(gWindow); // Destroy window
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


// Loads an image from file, converts it to a texture, and sets the texture's width/height
bool loadFromFile(LTexture* lTexture, const char* path) {
    freeTexture(lTexture); // Free existing texture

    SDL_Texture* newTexture = NULL;

    // Load image as surface
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        // Set color key (transparent) for the loaded image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        } else {
            lTexture->width = loadedSurface->w;
            lTexture->height = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface); // Free loaded surface
    }

    lTexture->texture = newTexture;
    return lTexture->texture != NULL;
}

// Renders text as a texture, stores it in the given LTexture struct
bool loadFromRenderedText(LTexture* lTexture, const char* textureText, SDL_Color textColor) {
    freeTexture(lTexture); // Free existing texture

    SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, textureText, textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        lTexture->texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (lTexture->texture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            lTexture->width = textSurface->w;
            lTexture->height = textSurface->h;
        }
        SDL_FreeSurface(textSurface); // Free surface
    }
    return lTexture->texture != NULL;
}



// Frees texture memory if it exists
void freeTexture(LTexture* lTexture) {
    if (lTexture->texture != NULL) {
        SDL_DestroyTexture(lTexture->texture);
        lTexture->texture = NULL;
        lTexture->width = 0;
        lTexture->height = 0;
    }
}

// Renders a texture with optional clipping, rotation, and flipping
void renderTexture(LTexture* lTexture, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect renderQuad = {x, y, lTexture->width, lTexture->height}; // Set rendering space

    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(gRenderer, lTexture->texture, clip, &renderQuad, angle, center, flip);
}

// Gets the width of a texture
int getTextureWidth(LTexture* lTexture) {
    return lTexture->width;
}

// Gets the height of a texture
int getTextureHeight(LTexture* lTexture) {
    return lTexture->height;
}


void buttonDrawer(SDL_Renderer* renderer, SDL_Rect buttons[], int count) { //renders the buttons NOT DRAW THEM

    
    SDL_SetRenderDrawColor(renderer, 105, 105, 105, 255); // Button color
    for (int i = 0; i < count; i++) {
        SDL_RenderFillRect(renderer, &buttons[i]); // Draw each button


    }
}

void grid(){ //line grid used for button outlines and canvas of the calculator
    int spacer = 0; 
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // line color

    for (int i = 0; i < 3; i++)
    {
        SDL_RenderDrawLine(gRenderer, BUTTON_WIDTH + spacer, SCREEN_HEIGHT, BUTTON_WIDTH + spacer, 90); 
        spacer += BUTTON_WIDTH;
    }
    spacer = 60; 

    for (int i = 0; i < 6; i++)
    {
        SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT - spacer, SCREEN_WIDTH, SCREEN_HEIGHT - spacer); 
        spacer += BUTTON_HEIGHT;
    }
}


char whichButtonWasPressed(int buttonX, int buttonY) {
    // Row 1 (Y = 390, Characters: "X", "0", ".")
    if (buttonY >= 390 && buttonY < 450) {
        if (buttonX >= 0 && buttonX < 83) {
            BUTTON_TO_CHANGE_COLOR = 0; 
            return 'X';
        } else if (buttonX >= 83 && buttonX < 166) {
            BUTTON_TO_CHANGE_COLOR = 1; 
            return '0';
        } else if (buttonX >= 166 && buttonX < 249) {
            BUTTON_TO_CHANGE_COLOR = 2; 
            return '.';
        }
    }

    // Row 2 (Y = 330, Characters: "1", "2", "3")
    else if (buttonY >= 330 && buttonY < 390) {
        if (buttonX >= 0 && buttonX < 83) {
            BUTTON_TO_CHANGE_COLOR = 3; 
            return '1';
        } else if (buttonX >= 83 && buttonX < 166) {
            BUTTON_TO_CHANGE_COLOR = 4; 
            return '2';
        } else if (buttonX >= 166 && buttonX < 249) {
            BUTTON_TO_CHANGE_COLOR = 5; 
            return '3';
        }
    }

    // Row 3 (Y = 270, Characters: "4", "5", "6")
    else if (buttonY >= 270 && buttonY < 330) {
        if (buttonX >= 0 && buttonX < 83) {
            BUTTON_TO_CHANGE_COLOR = 6; 
            return '4';
        } else if (buttonX >= 83 && buttonX < 166) {
            BUTTON_TO_CHANGE_COLOR = 7; 
            return '5';
        } else if (buttonX >= 166 && buttonX < 249) {
            BUTTON_TO_CHANGE_COLOR = 8; 
            return '6';
        }
    }

    // Row 4 (Y = 210, Characters: "7", "8", "9")
    else if (buttonY >= 210 && buttonY < 270) {
        if (buttonX >= 0 && buttonX < 83) {
            BUTTON_TO_CHANGE_COLOR = 9; 
            return '7';
        } else if (buttonX >= 83 && buttonX < 166) {
            BUTTON_TO_CHANGE_COLOR = 10; 
            return '8';
        } else if (buttonX >= 166 && buttonX < 249) {
            BUTTON_TO_CHANGE_COLOR = 11; 
            return '9';
        }
    }

    // Row 5 (Y = 150, Characters: "+", "-", "*")
    else if (buttonY >= 150 && buttonY < 210) {
        if (buttonX >= 0 && buttonX < 83) {
            BUTTON_TO_CHANGE_COLOR = 12; 
            return '+';
        } else if (buttonX >= 83 && buttonX < 166) {
            BUTTON_TO_CHANGE_COLOR = 13; 
            return '-';
        } else if (buttonX >= 166 && buttonX < 249) {
            BUTTON_TO_CHANGE_COLOR = 14; 
            return '*';
        }
    }

    // Row 6 (Y = 90, Characters: "%", "CE", "/")
    else if (buttonY >= 90 && buttonY < 150) {
        if (buttonX >= 0 && buttonX < 83) {
            BUTTON_TO_CHANGE_COLOR = 15; 
            return '=';
        } else if (buttonX >= 83 && buttonX < 166) {
            BUTTON_TO_CHANGE_COLOR = 16; 
            return 'C'; // Assuming 'CE' can be returned as 'C'
        } else if (buttonX >= 166 && buttonX < 249) {
            BUTTON_TO_CHANGE_COLOR = 17; 
            return '/';
        }
    }

    // Row 7 (Y = 0-90, when no button was pressed but a left click was registered)
    else if (buttonY >= 0 && buttonY < 90) {
        if (buttonX >= 0 && buttonX < 249) {
            BUTTON_TO_CHANGE_COLOR = -1; // Indicates no specific button
            return '!'; // Used to fix bug
        } 
    }

    // If no button was pressed
    return -1;
}

void whichOperatorWasUsed(char * presented){ // main calculator logic, very basic, only works with 2 numbers, I'll

    float beforeOp; // the numbers before and after the operator
    float afterOp;
    float answer = 0; //answer for the 2 numbers 
    int length = strlen(presented); 

    for (int i = 1; i < length; i++){
    
        if (presented[i] == '*'){
            sscanf(presented, "%f*%f", &beforeOp, &afterOp); 
            answer = beforeOp * afterOp; 
            sprintf(presented, "%.2f", answer); 
        }


        else if (presented[i] == '+'){
            sscanf(presented, "%f+%f", &beforeOp, &afterOp); 
            answer = beforeOp + afterOp; 
            sprintf(presented, "%.2f", answer); 
        }

        else if (presented[i] == '-'){
            
            sscanf(presented, "%f-%f", &beforeOp, &afterOp); 
            answer = beforeOp - afterOp; 
            sprintf(presented, "%.2f", answer); 
        }

        else if (presented[i] == '/'){
            sscanf(presented, "%f/%f", &beforeOp, &afterOp); 
            answer = beforeOp / afterOp; 
            sprintf(presented, "%.2f", answer); 
        }
                                    
    }


}

// very dumb function with too many parameters just to change button color on press
void colorChanger(int buttonX, int buttonY, SDL_Renderer* renderer, SDL_Rect buttons[], int R, int G, int B, int whichButton){
    SDL_SetRenderDrawColor(renderer, R, G, B, 255); // Button color
    SDL_RenderFillRect(renderer, &buttons[whichButton]); // Draw each button

}

void removeLetter(char * str, char charToRemove){

    int length = strlen(str); 

    for (int i = 0; i < length; i++)
    {

        if (str[i] == charToRemove)
        {
            for (int j = i; j < length; j++)
            {
                str[j] = str[j+1]; 
            }
        }
        
        str[length] = '\0'; 

    }

}







// This is where the magic happens...
// Main function - sets up SDL, loads media, runs main loop, and cleans up
int main(int argc, char* args[]) {
    if (!init()) { // Initialize SDL and create window
        printf("Failed to initialize!\n");
    } 
    else {
        if (!loadMedia()) { // Load media (text textures, fonts)
            printf("Failed to load media!\n");
        } 
        else {
            int quit = 0; // Main loop flag
            SDL_Event event; // Event handler

            SDL_Rect buttons[NUM_BUTTONS]; 
            int spacerHor = 0; // these space the buttons
            int spacerVer = 60; 
            int exactButton = 0; //this is stupid but this gives memory to the inner for loop

            for (int c = 0; c < 6; c++) //this places the buttons in the correct positions with thee correct sizes
            {
                for (int i = 0; i < 3; i++)
                {
                    buttons[i+exactButton].w = BUTTON_WIDTH; 
                    buttons[i+exactButton].h = BUTTON_HEIGHT; 
                    buttons[i+exactButton].y = SCREEN_HEIGHT - spacerVer; 
                    buttons[i+exactButton].x = spacerHor; 
                    

                    spacerHor += 83; 
                }
                spacerHor = 0;
                spacerVer+=60;
                exactButton+=3;
                
            }



            


            //these are used for calculator logic            
            char presented[32] = "\0"; //what will be presented
            float beforeOp; // the numbers before and after the operator
            float afterOp;
            float answer = 0; //answer for the 2 numbers 

            // used for changing color on mouse down
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            int R, G, B; 


            // these are for checking / and *
            int mulWasUsed = -1; // these are for weather or not * && / were used
            int posMul = -1; // these are for which was used last
            int divWasUsed = -1; // these are for weather or not * && / were used
            int posDiv = -1; // these are for which was used last

            int mulTimes = 0; // these are for how many of * or / were used - so how many times to run the loop
            int divTimes = 0; // these are for how many of * or / were used - so how many times to run the loop
            // these are for checking / and *



            while (!quit) {

                while (SDL_PollEvent(&event) != 0) { // Handle events

                    if (event.type == SDL_QUIT) quit = 1; // User requests quit
                    

                    if (event.type == SDL_KEYDOWN) {
                      if (event.key.keysym.sym == SDLK_ESCAPE) quit = true; // Exit on pressing the escape key
                    }

                    //mouse stuff
                    switch (event.type) {
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT) {

                                //pretty simple and intuative code below

                                char button = whichButtonWasPressed(event.button.x, event.button.y); 
                                mouseX = event.button.x;
                                mouseY = event.button.y;
                                R = G = B = 192;  

                                
                                
                                
                                if (button != 'X' && button != 'C' && button != '=' && button != '!')
                                {
                                    char temp[2] = { button, '\0' }; // Convert char to string format
                                    strcat(presented, temp);
                                }
                                

                                if (button == 'C') presented[0] = '\0'; //clears the text

                                if (button == 'X') presented[strlen(presented) - 1] = '\0'; //deletes the final letter


                                // this below is stupid but its meant to check if user enters * and /
                                for (int i = 0; i < 32; i++)
                                {
                                    if (presented[i] == '*')
                                    {
                                        mulWasUsed = 1; 
                                        posMul = i;
                                        mulTimes++;

                                    }

                                    if (presented[i] == '/')
                                    {
                                        divWasUsed = 1;  
                                        posDiv = i;
                                        divTimes++; 
                                
                                    }
                                    
                                    
                                }

                                if (posDiv > posMul)
                                {

                                    for (int i = 0; i < divTimes; i++)
                                    {
                                        removeLetter(presented, '*'); 
                                    }
                                    
                                }
                                else if (posDiv < posMul)
                                {

                                    for (int i = 0; i < mulTimes; i++)
                                    {
                                        removeLetter(presented, '/'); 
                                    }
                                }
                                // this above is stupid but its meant to check if user enters * and /



                                if (button == '=') //returns answer
                                    whichOperatorWasUsed(presented); 

                                if (button != -1 && strlen(presented) > 0) //used to handle every button except X and CE
                                    loadFromRenderedText(&inputLine, presented, textColor);

                                else if(button != -1 && strlen(presented) == 0) //used to handle X and C
                                    loadFromRenderedText(&inputLine, " ", textColor); 

                                // reset the variables... there is got to be a better way to do this...
                                mulWasUsed = -1;
                                posMul = -1; 
                                divWasUsed = -1;
                                posDiv = - 1;
                                mulTimes = 0;
                                divTimes = 0;
                                


                            }
                            break;
                        case SDL_MOUSEBUTTONUP:
                            R = G = B = 105;  
                            // change color again once the button is released
                            break;
                        case SDL_MOUSEMOTION:
                            // change button color on hover
                            mouseX = event.motion.x;
                            mouseY = event.motion.y;
                            char unnecessaryHolder = whichButtonWasPressed(event.motion.x, event.motion.y); 

                            R = G = B = 72;  
                            

                            break;
                    }



                    
                }

                // Clear screen with white background
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);

        

                buttonDrawer(gRenderer, buttons, NUM_BUTTONS); //this renders the buttons rather than draws them
                colorChanger(mouseX, mouseY, gRenderer, buttons, R, G, B, BUTTON_TO_CHANGE_COLOR); // this changes button color on press
                grid(); // this will draw the grid of lines

                

                
                renderTexture(&inputLine, 0,20, NULL, 0, NULL, SDL_FLIP_NONE); //this is for user input text (dk, posx, posy, dw, dw, dw,dw); 

                for (int i = 0; i < NUM_BUTTONS; i++) { // this draws the text of each button in the centre of each button
                    SDL_Rect quack = {buttons[i].x + BUTTON_MID_X, buttons[i].y + BUTTON_MID_Y, allOfTheButtonsTextTextures[i].width, allOfTheButtonsTextTextures[i].height};
                    renderTexture(&allOfTheButtonsTextTextures[i], quack.x, quack.y, NULL, 0, NULL, SDL_FLIP_NONE);
                }

                SDL_RenderPresent(gRenderer); // Update screen
                
            }
        }
    }
    close(); // Free resources and close SDL

    return 0;
}
