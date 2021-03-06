#include <SDL.h>
#include <iostream>
#include "renderer.h"
#include "level.h"
#include "game_object.h"
#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

void game(renderer& r) {
    
    // Disable the cursor
    // SDL_ShowCursor(SDL_DISABLE);

    float cam_height = 40.0f;

    game_object* player;
    level l;
    l.view.set_position(0.0f, 0.0f, cam_height);
    //l.view.set_orientation(230.0f, 1.0f, 0.0f, 0.0f);

    game_object& cube = l.new_game_object();
    cube.set_colour(1.0f, 0.0f, 0.0f);
    player = &cube;


    // Ground
    {
        game_object& ground = l.new_game_object();
        ground.set_position(0, 0, -1); // one below player
        ground.set_scale(100, 100, 1);
        ground.set_colour(0, 0.6, 0);

        for (int i = 0; i < 5; ++i)
        {
            game_object& stump = l.new_game_object();
            stump.set_position(5, -5 + i * 3, 0.0f);
            stump.set_colour(0.2, 0.1, 0);

            game_object& bush = l.new_game_object();
            bush.set_scale(2, 2, 3);
            bush.set_position(5,-5 + i * 3, 2);
            bush.set_colour(0.0, 0.4, 0);
        }
    }

    r.set_level(l);

    SDL_Event event;
    bool quit = false;
    enum DOWN_KEYS {W_DOWN, A_DOWN, S_DOWN, D_DOWN };
    bool key_down[4] = {false};
    int center_x, center_y;
    r.get_render_size(center_x, center_y);
    center_x /= 2;
    center_y /= 2;
    glm::vec3 current_position;
    glm::vec4 mouse;
    glm::mat4 inv_view = l.view.get_inv_view();
    float angY = 0.0f;
    float angX = 0.0f;

    while ( !quit ) {      
        while( SDL_PollEvent( &event ) ){
            switch (event.type) {      
              case SDL_KEYDOWN:     
                if ( event.key.keysym.sym == SDLK_ESCAPE )
                    quit = true;
                if(player) {
                    if ( event.key.keysym.sym == SDLK_w )
                        key_down[W_DOWN] = true;
                    if ( event.key.keysym.sym == SDLK_a )
                        key_down[A_DOWN] = true;
                    if ( event.key.keysym.sym == SDLK_s )
                        key_down[S_DOWN] = true;
                    if ( event.key.keysym.sym == SDLK_d )
                        key_down[D_DOWN] = true;
                }
                break;
            case SDL_KEYUP:     
                if(player) {
                    if ( event.key.keysym.sym == SDLK_w )
                        key_down[W_DOWN] = false;
                    if ( event.key.keysym.sym == SDLK_a )
                        key_down[A_DOWN] = false;
                    if ( event.key.keysym.sym == SDLK_s )
                        key_down[S_DOWN] = false;
                    if ( event.key.keysym.sym == SDLK_d )
                        key_down[D_DOWN] = false;
                }
                break;
             case SDL_MOUSEMOTION:

                // get the vector from the players current position
                current_position = player->get_position();
                mouse = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
                mouse.x = event.motion.x / (float)center_x - 1.0f;
                mouse.y = (event.motion.y / (float)center_y - 1.0f) * -1.0f;
                mouse =  inv_view * mouse;
                mouse = mouse / mouse.w;
                angY = atan(mouse.y / (cam_height - mouse.z));
                angX = atan(mouse.x / (cam_height - mouse.z));

                mouse.y = tan(angY) * cam_height;
                mouse.x = tan(angX) * cam_height;

                std::cout << "CUR_POS (X, Z): " << current_position.x << ", " << current_position.y << std::endl;
                std::cout << "Mouse Position (X, Y): "<< mouse.x << ", "<< mouse.y << ", " << mouse.z << ", " << mouse.w << std::endl;
                std::cout << "Difference (X, Z): " << current_position.x - event.motion.x << ", " << current_position.y - event.motion.y << std::endl;

                player->set_position(mouse.x, mouse.y, 0.0f);
                
                // player->yaw((center_x - event.motion.x)/10.0f);
                // player->pitch(-(center_y - event.motion.y)/10.0f);
                break;
             case SDL_QUIT:
                quit = true;
                break;
            }
        }


        if ( key_down[W_DOWN])
            player->move_forward(0.2f);
        if ( key_down[A_DOWN] )
            player->move_left(0.2f);
        if ( key_down[S_DOWN] )
            player->move_backward(0.2f);
        if ( key_down[D_DOWN] )
            player->move_right(0.2f);

        current_position = player->get_position();

        //boundaries
        // if(current_position.x > 20.0f)
        //     player->set_x(20.0f);
        // if(current_position.x < -20.0f)
        //     player->set_x(-20.0f);
        // if(current_position.y > 12.0f)
        //     player->set_y(12.0f);
        // if(current_position.y < -12.0f)
        //     player->set_y(-12.0f);


        // Stop mouse from going outside of the window
        // SDL_WarpMouseInWindow(r.get_window(), center_x, center_y);

        r.render();
    }
}
 
int main(int argc, char *argv[])
{
    renderer r;

    game(r);

    return 0;
}