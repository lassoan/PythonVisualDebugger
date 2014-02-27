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

#ifndef __qSlicerPythonVisualDebuggerModuleWidget_h
#define __qSlicerPythonVisualDebuggerModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerPythonVisualDebuggerModuleExport.h"

class qSlicerPythonVisualDebuggerModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_PYTHONVISUALDEBUGGER_EXPORT qSlicerPythonVisualDebuggerModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerPythonVisualDebuggerModuleWidget(QWidget *parent=0);
  virtual ~qSlicerPythonVisualDebuggerModuleWidget();

public slots:

  void onStartServerButtonClicked();

protected:
  QScopedPointer<qSlicerPythonVisualDebuggerModuleWidgetPrivate> d_ptr;
  
  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerPythonVisualDebuggerModuleWidget);
  Q_DISABLE_COPY(qSlicerPythonVisualDebuggerModuleWidget);
};

#endif
