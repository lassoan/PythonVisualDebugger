/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerPythonVisualDebuggerFooBarWidget.h"
#include "ui_qSlicerPythonVisualDebuggerFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PythonVisualDebugger
class qSlicerPythonVisualDebuggerFooBarWidgetPrivate
  : public Ui_qSlicerPythonVisualDebuggerFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerPythonVisualDebuggerFooBarWidget);
protected:
  qSlicerPythonVisualDebuggerFooBarWidget* const q_ptr;

public:
  qSlicerPythonVisualDebuggerFooBarWidgetPrivate(
    qSlicerPythonVisualDebuggerFooBarWidget& object);
  virtual void setupUi(qSlicerPythonVisualDebuggerFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerPythonVisualDebuggerFooBarWidgetPrivate
::qSlicerPythonVisualDebuggerFooBarWidgetPrivate(
  qSlicerPythonVisualDebuggerFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerPythonVisualDebuggerFooBarWidgetPrivate
::setupUi(qSlicerPythonVisualDebuggerFooBarWidget* widget)
{
  this->Ui_qSlicerPythonVisualDebuggerFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerPythonVisualDebuggerFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerPythonVisualDebuggerFooBarWidget
::qSlicerPythonVisualDebuggerFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerPythonVisualDebuggerFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerPythonVisualDebuggerFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerPythonVisualDebuggerFooBarWidget
::~qSlicerPythonVisualDebuggerFooBarWidget()
{
}
