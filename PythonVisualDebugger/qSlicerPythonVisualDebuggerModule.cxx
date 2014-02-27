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
#include <QtPlugin>

// PythonVisualDebugger Logic includes
#include <vtkSlicerPythonVisualDebuggerLogic.h>

// PythonVisualDebugger includes
#include "qSlicerPythonVisualDebuggerModule.h"
#include "qSlicerPythonVisualDebuggerModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerPythonVisualDebuggerModule, qSlicerPythonVisualDebuggerModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPythonVisualDebuggerModulePrivate
{
public:
  qSlicerPythonVisualDebuggerModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPythonVisualDebuggerModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerPythonVisualDebuggerModulePrivate
::qSlicerPythonVisualDebuggerModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPythonVisualDebuggerModule methods

//-----------------------------------------------------------------------------
qSlicerPythonVisualDebuggerModule
::qSlicerPythonVisualDebuggerModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerPythonVisualDebuggerModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerPythonVisualDebuggerModule::~qSlicerPythonVisualDebuggerModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerPythonVisualDebuggerModule::helpText()const
{
  return "This is a loadable module bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerPythonVisualDebuggerModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPythonVisualDebuggerModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerPythonVisualDebuggerModule::icon()const
{
  return QIcon(":/Icons/PythonVisualDebugger.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerPythonVisualDebuggerModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPythonVisualDebuggerModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerPythonVisualDebuggerModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerPythonVisualDebuggerModule
::createWidgetRepresentation()
{
  return new qSlicerPythonVisualDebuggerModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerPythonVisualDebuggerModule::createLogic()
{
  return vtkSlicerPythonVisualDebuggerLogic::New();
}
