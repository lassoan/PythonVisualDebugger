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

// PythonVisualDebugger Logic includes
#include "vtkSlicerPythonVisualDebuggerLogic.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

// STD includes
#include <cassert>

#include "hapdbg/hapdbg.h"

#define HE_DEBUGGER_PORTNUM	2020


//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerPythonVisualDebuggerLogic);

//----------------------------------------------------------------------------
vtkSlicerPythonVisualDebuggerLogic::vtkSlicerPythonVisualDebuggerLogic()
{
  this->ConnectedToRemoteDebugger=false;
}

//----------------------------------------------------------------------------
vtkSlicerPythonVisualDebuggerLogic::~vtkSlicerPythonVisualDebuggerLogic()
{
  hapdbg::Uninitialize();
}

//----------------------------------------------------------------------------
void vtkSlicerPythonVisualDebuggerLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerPythonVisualDebuggerLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());

  int nPort = HE_DEBUGGER_PORTNUM;
  if ( ! hapdbg::Initialize(nPort))
  {
    vtkErrorMacro("Failed to initialize Python visual debugger");		
  }
}

//-----------------------------------------------------------------------------
void vtkSlicerPythonVisualDebuggerLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerPythonVisualDebuggerLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerPythonVisualDebuggerLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerPythonVisualDebuggerLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}


//---------------------------------------------------------------------------
void vtkSlicerPythonVisualDebuggerLogic::Test()
{

}

//---------------------------------------------------------------------------
void vtkSlicerPythonVisualDebuggerLogic::HandleRemoteConnections()
{
  if (!hapdbg::HasConnection())
  {
    // no client is waiting to be connected, nothing to do
    this->ConnectedToRemoteDebugger=false;
    return;
  }
  if (this->ConnectedToRemoteDebugger)
  {
    // already connected, nothing to do
    return;
  }
  this->ConnectedToRemoteDebugger=hapdbg::CheckForNewConnection();
  if (this->ConnectedToRemoteDebugger)
  {
    vtkDebugMacro("Python visual debugger attached");
  }
}
