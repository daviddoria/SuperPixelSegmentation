/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "HelpersQt.h"

// Qt
#include <QTableWidget>
#include <QGraphicsView>

namespace HelpersQt
{

void QColorToUCharColor(const QColor& color, unsigned char outputColor[3])
{
  outputColor[0] = color.red();
  outputColor[1] = color.green();
  outputColor[2] = color.blue();
}

QImage FitToGraphicsView(const QImage qimage, const QGraphicsView* gfx)
{
  // The fudge factors so that the scroll bars do not appear
  //std::cout << "Fitting to width: " << gfx->width() << " height: " << gfx->height() << std::endl;
  
  unsigned int fudge = 6;
  if(gfx->height() < gfx->width())
    {
    return qimage.scaledToHeight(gfx->height() - fudge);
    }
  else
    {
    return qimage.scaledToWidth(gfx->width() - fudge);
    }
}



} // end namespace
