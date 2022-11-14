import sys
from PyQt5.QtWidgets import QMainWindow, QApplication ,QFileDialog
from test2 import Ui_MainWindow
#from slider import Ui_MainWindow
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor
import vtk
from vtk.util.misc import vtkGetDataRoot



surfaceExtractor = vtk.vtkContourFilter()
volumeMapper = vtk.vtkGPUVolumeRayCastMapper()
mode = 0


    
class AppWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.mode = 0
        self.iso = 0
        self.tissue_index = 0
        self.red = [1,1,1]
        self.green = [0.5 ,0.5 ,1 ]
        self.blue = [0.3 , 0.3, 0.9]
        self.opacity = [0.15 ,0.15 ,0.85]
        self.path = 'data/Head'
        self.ui.horizontalSlider_5.sliderReleased.connect(lambda: self.Set_iso())
        self.ui.horizontalSlider.sliderReleased.connect(lambda: self.Set_Red())
        self.ui.horizontalSlider_2.sliderReleased.connect(lambda: self.Set_green())
        self.ui.horizontalSlider_3.sliderReleased.connect(lambda: self.Set_blue())
        self.ui.horizontalSlider_4.sliderReleased.connect(lambda: self.Set_opacity())
        self.ui.comboBox.currentIndexChanged.connect(lambda: self.SetMode())
        self.ui.comboBox_2.currentIndexChanged.connect(lambda: self.SetTissueIndex())
        self.ui.pushButton.clicked.connect(lambda: self.OpenFilePath())

        #self.ui.MatchButton.clicked.connect(MatchSong)

        self.show()  
    
    def SetMode(self):
        self.mode=self.ui.comboBox.currentIndex()
        vtk_rendering(self.return_Data())


    def OpenFilePath(self):
        self.path = QFileDialog.getExistingDirectory()
        vtk_rendering(self.return_Data())



    def SetTissueIndex(self):
        self.tissue_index=self.ui.comboBox_2.currentIndex()
        vtk_rendering(self.return_Data())

    def Set_iso(self):
        self.iso = self.ui.horizontalSlider_5.value()*1.5
        vtk_rendering(self.return_Data())

    def Set_Red(self):
        self.red[self.tissue_index]=self.ui.horizontalSlider.value()/100
        vtk_rendering(self.return_Data())

    def Set_green(self):
        self.green[self.tissue_index]=self.ui.horizontalSlider_2.value()/100
        vtk_rendering(self.return_Data())

    def Set_blue(self):
        self.blue[self.tissue_index]=self.ui.horizontalSlider_3.value()/100
        vtk_rendering(self.return_Data())

    def Set_opacity(self):
        self.opacity[self.tissue_index]=self.ui.horizontalSlider_4.value()/100
        vtk_rendering(self.return_Data())

    def return_Data(self):
        return self.mode , self.iso , self.tissue_index , self.red , self.green , self.blue ,self.opacity ,self.path


def vtk_rendering(Data):
    
    mode = Data[0]
    iso = Data[1]
    tissue = Data[2]
    red = Data[3]
    green= Data[4]
    blue =Data[5]
    opacity = Data[6]
    path = Data[7]
    renWin = iren.GetRenderWindow()
    aRenderer = vtk.vtkRenderer()
    renWin.AddRenderer(aRenderer)

    
    Dicom_reader = vtk.vtkDICOMImageReader()
    Dicom_reader.SetDataByteOrderToLittleEndian()
    Dicom_reader.SetDirectoryName(path)
    Dicom_reader.Update()

    if mode == 1:
        surfaceExtractor.SetInputConnection(Dicom_reader.GetOutputPort())
        surfaceRendering(aRenderer,iso)
    elif mode == 2:
        volumeMapper.SetInputConnection(Dicom_reader.GetOutputPort())
        RayCasting(aRenderer, tissue,red,green,blue,opacity)

    renWin.SetSize(640, 480)

    iren.Initialize()
    renWin.Render()
    iren.Start()
    iren.show()
     

def surfaceRendering(aRenderer,iso):

    surfaceExtractor.SetValue(0, iso)
    surfaceNormals = vtk.vtkPolyDataNormals()
    surfaceNormals.SetInputConnection(surfaceExtractor.GetOutputPort())
    surfaceNormals.SetFeatureAngle(60.0)
    surfaceMapper = vtk.vtkPolyDataMapper()
    surfaceMapper.SetInputConnection(surfaceNormals.GetOutputPort())
    surfaceMapper.ScalarVisibilityOff()
    
    surface = vtk.vtkActor()
    surface.SetMapper(surfaceMapper)
    
    aCamera = vtk.vtkCamera()
    aCamera.SetViewUp(0, 0, -1)
    aCamera.SetPosition(0, 1, 0)
    aCamera.SetFocalPoint(0, 0, 0)
    aCamera.ComputeViewPlaneNormal()
    
    aRenderer.AddActor(surface)
    aRenderer.SetActiveCamera(aCamera)
    aRenderer.ResetCamera()
    
    aRenderer.SetBackground(0, 0, 0)
    
    aRenderer.ResetCameraClippingRange()



def RayCasting(ren ,tissue,red,green,blue,opacity):

    volumeMapper.SetBlendModeToComposite()
    volumeColor = vtk.vtkColorTransferFunction()
    volumeColor.AddRGBPoint(0,    0.0, 0.0, 0.0)
    volumeColor.AddRGBPoint(1,  red[0], green[0], blue[0])
    volumeColor.AddRGBPoint(50, red[1], green[1], blue[1])
    volumeColor.AddRGBPoint(1000, red[2], green[2], blue[2])
    volumeScalarOpacity = vtk.vtkPiecewiseFunction()
    volumeScalarOpacity.AddPoint(0,    0.0)
    volumeScalarOpacity.AddPoint(1,  opacity[0])
    volumeScalarOpacity.AddPoint(50, opacity[1])
    volumeScalarOpacity.AddPoint(1000, opacity[2])
    volumeGradientOpacity = vtk.vtkPiecewiseFunction()
    volumeGradientOpacity.AddPoint(0,   0.0)
    volumeGradientOpacity.AddPoint(90,  0.5)
    volumeGradientOpacity.AddPoint(100, 1.0)
    volumeProperty = vtk.vtkVolumeProperty()
    volumeProperty.SetColor(volumeColor)
    volumeProperty.SetScalarOpacity(volumeScalarOpacity)
    volumeProperty.SetGradientOpacity(volumeGradientOpacity)
    volumeProperty.SetInterpolationTypeToLinear()
    volumeProperty.ShadeOn()
    volumeProperty.SetAmbient(0.4)
    volumeProperty.SetDiffuse(0.6)
    volumeProperty.SetSpecular(0.2)

    
    volume = vtk.vtkVolume()
    volume.SetMapper(volumeMapper)
    volume.SetProperty(volumeProperty)

    ren.AddViewProp(volume)

    
    camera =  ren.GetActiveCamera()
    c = volume.GetCenter()
    camera.SetFocalPoint(c[0], c[1], c[2])
    camera.SetPosition(c[0] + 400, c[1], c[2])
    camera.SetViewUp(0, 0, -1)

    


app = QApplication(sys.argv)
# The class that connect Qt with VTK
iren = QVTKRenderWindowInteractor()
w = AppWindow()

vtk_rendering(w.return_Data())
w.show()
sys.exit(app.exec_())
# Start the event loop.
