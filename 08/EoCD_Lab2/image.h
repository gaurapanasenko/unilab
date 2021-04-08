#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
#import "julia.h"

#define COMP 4

using namespace std;

/**
 * @brief Тип кольору, задає інтенсивність кольору у вигляді цілого числа
 *        у діапазоні значень [0, 255]
 */
typedef unsigned char color_t;
/**
 * @brief Тип пікселя, задає що у пікселя буде 4 канали інтенсивності:
 *        красний, зелений, синій та прозорість.
 */
typedef color_t pixel_t[COMP];

/**
 * @brief Зберігає самі піскелі, та розмір картинки.
 */
class Image
{
public:
    /**
     * @brief Пікселі картинки.
     */
    shared_ptr<const pixel_t[]> data;
    /**
     * @brief Розмір картинки
     */
    const int size[2];

    /**
     * @brief Конструктор, який поеднує пікселі картинки, та її розмір у
     *        цей клас.
     * @param data пікселі картинки
     * @param size розмір картинки
     */
    Image(shared_ptr<const pixel_t[]> data, const int size[2]);
    /**
     * @brief Метод який генерує нову картинку множини Жуліа.
     * @param size розмір картинки
     * @param jul вхідні параметри, які будуть використовуватися для генерації
     * @return нова картинка множини Жуліа
     */
    static Image julia(const int size[2], const Julia& jul);
};


#endif // IMAGE_H
