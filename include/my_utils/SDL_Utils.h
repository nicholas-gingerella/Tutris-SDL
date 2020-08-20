#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <iostream>
#include <string>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace SDL_Utils
{
    // Resource directories
    const std::string IMG_DIR = "images";
    const std::string FONT_DIR = "fonts";


    // Get path for resources located int res/sub_dir
    // Project directory structure assumed to be the 
    // following:
    // bin/
    //   executable
    // res/
    //   lesson1/
    //   lesson2/
    //
    // Paths returned will be lessons/res/sub_dir
    std::string get_resource_path(const std::string &subDir = "")
    {
        const std::string RESOURCE_DIR_NAME = "resources";

        // Choose path separator based one platform (Windows vs Linux)
        #ifdef _WIN32
            const char PATH_SEP = '\\';
        #else
            const char PATH_SEP = '/';
        #endif

        // will hold resource path: lessons/res/
        // declared static so SDL_GetBasePath only needs to be called
        // once, after which the value of this variable will persist
        // for entirety of the program run
        static std::string baseRes;
        if (baseRes.empty())
        {
            // SDL_GetBasePath() returns NULL if it fails to retrieve
            // a path
            char *basePath = SDL_GetBasePath();
            if (basePath)
            {
                baseRes = basePath;
                SDL_free(basePath);
            }
            else
            {
                std::cerr << "Error getting resource path" << SDL_GetError() << std::endl;
                return "";
            }

            // Replace the last "bin/" with "res/" to get the resource path
            size_t pos = baseRes.rfind("bin");
            baseRes = baseRes.substr(0, pos) + RESOURCE_DIR_NAME + PATH_SEP;
        }

        // If a subdirectory is specified, append it to the base path
        // otherwise, just return the base path
        return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
    }    


    // SDL cleanup template
    // Recurses through the list of arguments to clean up, cleaning up
    // the first one in the list each iteration
    template<typename T, typename... Args>
    void cleanup(T *t, Args&&... args)
    {
        // cleanup first item in the list
        cleanup(t);

        // recurse to clean up the remaining arguments
        cleanup(std::forward<Args>(args)...);
    }


    // These specializations serve to free the passed argument and also provide the
    // base cases for the recursive call above, eg. when args is only a single item
    // one of the specializations below will be called by
    // cleanup(std::forward<Args>(args)...), ending the recursion
    // We also make it safe to pass nullptrs to handle situations where we
    // don't want to bother finding out which values failed to load (and thus are null)
    // but rather just want to clean everything up and let cleanup sort it out
    template<>
    inline void cleanup<SDL_Window>(SDL_Window *win)
    {
        if (!win)
        {
            return;
        }
        SDL_DestroyWindow(win);
    }


    template<>
    inline void cleanup<SDL_Renderer>(SDL_Renderer *ren)
    {
        if (!ren)
        {
            return;
        }
        SDL_DestroyRenderer(ren);
    }


    template<>
    inline void cleanup<SDL_Texture>(SDL_Texture *tex)
    {
        if (!tex)
        {
            return;
        }
        SDL_DestroyTexture(tex);
    }


    template<>
    inline void cleanup<SDL_Surface>(SDL_Surface *surf)
    {
        if (!surf)
        {
            return;
        }
        SDL_FreeSurface(surf);
    }


    // Log an SDL error with some error message to the output stream
    // of our choice.
    // @param os The output stream to write the message to
    // @param msg The error message to write, format will be
    // "msg error: SDL_GetError()"
    void logSDLError(std::ostream &os, const std::string &msg)
    {
        os << msg << SDL_GetError() << std::endl;
    }


    // Loads an image into a texture on the rendering device
    // @param file The image file to load
    // @param ren The renderer to load the texture onto
    // @return the loaded texture, or nullptr if something went wrong
    SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
    {
        SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
        if (texture == nullptr)
        {
            logSDLError(std::cout, "LoadTexture");
        }
        return texture;
    }


    // Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
    // width and height
    // @param tex The source texture we want to draw
    // @param ren The renderer we want to draw to
    // @param x The x coordinate to draw to
    // @param y The y coordinate to draw to
    // @param w The width of the texture to draw
    // @param h The height of the texture to draw
    void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
        //Setup the destination rectangle to be at the position we want
        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(ren, tex, NULL, &dst);
    }


    // Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
    // the texture's width and height
    // @param tex The source texture we want to draw
    // @param ren The renderer we want to draw to
    // @param dst The destination rectangle to render th texture to
    // @param clip The sub-section of the texture to draw (clipping rect)
    void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr)
    {
        SDL_RenderCopy(ren, tex, clip, &dst);
    }


    // Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
    // the texture's width and height
    // @param tex The source texture we want to draw
    // @param ren The renderer we want to draw to
    // @param x The x coordinate to draw to
    // @param y The y coordinate to draw to
    void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
    {
        int w, h;
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        renderTexture(tex, ren, x, y, w, h);
    }


    // Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
    // the texture's width and height and taking a clip of the texture if
    // if a clip rectangle is passed. If a clip is present, the clip's width
    // and height will be used in place of the texture's dimensions
    // @param tex The source texture we want to draw
    // @param ren The renderer we want to draw to
    // @param x The x coordinate to draw to
    // @param y The y coordinate to draw to
    // @param clip The sub-section of the texture to draw (clipping rect)
    //             default of nullptr draws the entire texture
    // void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr)
    // {
    //     SDL_Rect dst;
    //     dst.x = x;
    //     dst.y = y;
    //     if (clip != nullptr)
    //     {
    //         dst.w = clip->w;
    //         dst.h = clip->h;
    //     }
    //     else
    //     {
    //         SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    //     }

    //     renderTexture(tex, ren, dst, clip);    
    // }

    // Render the message we want to display to a texture for drawing
    // @param message The message we want to display
    // @param fontFile The font we want to use to render the text
    // @param color The color we want the text to be
    // @param fontSize The size we want the font to be
    // @param renderer The renderer to load the texture in
    // @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
    SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
    {
        // Open the font
        TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
        if (font == nullptr)
        {
            logSDLError(std::cout, "TTF_OpenFont");
            return nullptr;
        }

        // We need to first render to a surface as that's what TTF_RenderText
        // returns, then load that surface into a texture
        SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
        if (surf == nullptr)
        {
            TTF_CloseFont(font);
            logSDLError(std::cout, "TTF_RenderText");
            return nullptr;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
        if (texture == nullptr)
        {
            logSDLError(std::cout, "CreateTexture");
        }

        // Clean up the surface and font
        SDL_FreeSurface(surf);
        TTF_CloseFont(font);
        return texture;
    }

} // end SDL_UTILS namespace

#endif