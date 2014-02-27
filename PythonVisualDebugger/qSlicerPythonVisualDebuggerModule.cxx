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
#include <QTimer>

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
  
  // We use a timer-based polling mechanism to manage the socket communication from the main thread
  // without blocking the main thread
  QTimer RemoteConnectionHandlerTimer;
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
  Q_D(qSlicerPythonVisualDebuggerModule);
  connect(&d->RemoteConnectionHandlerTimer, SIGNAL(timeout()), this, SLOT(handleRemoteConnections()));
  d->RemoteConnectionHandlerTimer.start(1000);
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
  return "This work was was partially funded by SparKit and OCAIRO";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPythonVisualDebuggerModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Andras Lasso (PerkLab, Queen's)");
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
  return QStringList() << "Development";
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

//-----------------------------------------------------------------------------
void qSlicerPythonVisualDebuggerModule::handleRemoteConnections()
{
  vtkMRMLAbstractLogic* l = this->logic();
  vtkSlicerPythonVisualDebuggerLogic * pythonDebuggerLogic = vtkSlicerPythonVisualDebuggerLogic::SafeDownCast(l);
  if (pythonDebuggerLogic)
    {
    pythonDebuggerLogic->HandleRemoteConnections();
    }
}
