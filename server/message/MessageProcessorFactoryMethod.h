#ifndef MESSAGEPROCESSORFACTORYMETHOD_H
#define MESSAGEPROCESSORFACTORYMETHOD_H

#include <memory>
#include "MessageProcessorBase.h"

/* SpMessageProcessorBase get_message_processor();
 *
 * funtion with such signature returns shared pointer to concrete MessageProcessor
 * it should exist in each plugin library */

typedef SpMessageProcessorBase (*MessageProcessorFactoryMethod)(void);

#endif // MESSAGEPROCESSORFACTORYMETHOD_H
