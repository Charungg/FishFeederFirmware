//
// Created by Charlie Cheung on 12/12/2024.
//

#ifndef OPERATINGMODEENUM_H
#define OPERATINGMODEENUM_H

/**
 * Contains all the feeding system operating mode.
 */
typedef enum{
    AUTO = 'A',     // Signals the feeding schedule will continue automatically.
    PAUSE = 'P',    // Signals the feeding schedule will pause unless told otherwise.
    SKIP = 'S'      // Signal the feeding schedules will skip one schedule and continue in automatic.
}operatingEnum;

#endif //OPERATINGMODEENUM_H
