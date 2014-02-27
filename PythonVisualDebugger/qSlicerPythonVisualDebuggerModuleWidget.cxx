/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerPythonVisualDebuggerModuleWidget.h"
#include "ui_qSlicerPythonVisualDebuggerModuleWidget.h"
#include "vtkSlicerPythonVisualDebuggerLogic.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPythonVisualDebuggerModuleWidgetPrivate: public Ui_qSlicerPythonVisualDebuggerModuleWidget
{
  Q_DECLARE_PUBLIC( qSlicerPythonVisualDebuggerModuleWidget );  

protected:
  qSlicerPythonVisualDebuggerModuleWidget* const q_ptr;
public:
  qSlicerPythonVisualDebuggerModuleWidgetPrivate( qSlicerPythonVisualDebuggerModuleWidget& object );
  ~qSlicerPythonVisualDebuggerModuleWidgetPrivate();
 
  vtkSlicerPythonVisualDebuggerLogic* logic() const
  {
    Q_Q( const qSlicerPythonVisualDebuggerModuleWidget );
    return vtkSlicerPythonVisualDebuggerLogic::SafeDownCast( q->logic() );
  } 
};

//-----------------------------------------------------------------------------
// qSlicerPythonVisualDebuggerModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPythonVisualDebuggerModuleWidgetPrivate::qSlicerPythonVisualDebuggerModuleWidgetPrivate( qSlicerPythonVisualDebuggerModuleWidget& object ) : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
qSlicerPythonVisualDebuggerModuleWidgetPrivate::~qSlicerPythonVisualDebuggerModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPythonVisualDebuggerModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerPythonVisualDebuggerModuleWidget::qSlicerPythonVisualDebuggerModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerPythonVisualDebuggerModuleWidgetPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qSlicerPythonVisualDebuggerModuleWidget::~qSlicerPythonVisualDebuggerModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerPythonVisualDebuggerModuleWidget::setup()
{
  Q_D(qSlicerPythonVisualDebuggerModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  connect( d->PushButton_StartServer, SIGNAL( clicked() ), this, SLOT( onStartServerButtonClicked() ) );
}

//-----------------------------------------------------------------------------
void qSlicerPythonVisualDebuggerModuleWidget::onStartServerButtonClicked()
{
  Q_D(qSlicerPythonVisualDebuggerModuleWidget);
  qCritical() << "qSlicerPythonVisualDebuggerModuleWidget::onStartServerButtonClicked() called"; 
  d->logic()->StartServer(); 
}
