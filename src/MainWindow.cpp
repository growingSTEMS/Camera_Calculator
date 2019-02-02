#include "MainWindow.h"

extern OutputManager cout;

//private functions
void MainWindow::makeConnections()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: makeConnections()" << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  connect(m_ui->btnReset, &QPushButton::clicked, this, &MainWindow::btnResetClickHandler);
  connect(m_ui->btnCalculate, &QPushButton::clicked, 
          this, &MainWindow::btnCalculateClickHandler);
  connect(m_ui->btnClearHistory, &QPushButton::clicked, 
          m_ui->lstResults, &QListWidget::clear);

  connect(m_ui->actClose, &QAction::triggered, this, &MainWindow::close);
  connect(m_ui->actAbout, &QAction::triggered, this, &MainWindow::actAboutHandler);
}
void MainWindow::setup()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: setup()" << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  auto lengthValidator = new QDoubleValidator(0.0, 2000.0, 20);
  lengthValidator->setNotation(QDoubleValidator::Notation::StandardNotation);
  auto fovValidator = new QDoubleValidator(0.0, 360.0, 20);
  fovValidator->setNotation(QDoubleValidator::Notation::StandardNotation);
  auto orientValidator = new QDoubleValidator(-90.0, -90.0, 20);
  orientValidator->setNotation(QDoubleValidator::Notation::StandardNotation);

  m_ui->lneCamDist->setValidator(lengthValidator);
  m_ui->lneCamHeight->setValidator(lengthValidator);
  m_ui->lneCamOffset->setValidator(lengthValidator);
  m_ui->lneCamFovVert->setValidator(fovValidator);
  m_ui->lneCamFovHor->setValidator(fovValidator);
  m_ui->lneCamPitch->setValidator(orientValidator);
  m_ui->lneCamYaw->setValidator(orientValidator);
  m_ui->lneVtCenterHeight->setValidator(lengthValidator);
  m_ui->lneVtHeight->setValidator(lengthValidator);
  m_ui->lneVtWidth->setValidator(lengthValidator);
}
void MainWindow::reset()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: reset()" << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  //set all the values back to 0
  m_ui->lneCamPitch->clear();
  m_ui->lneCamYaw->clear();
  m_ui->lneVtWidth->clear();
  m_ui->lneVtHeight->clear();
  m_ui->lneCamFovHor->clear();
  m_ui->lneCamFovVert->clear();
  m_ui->lneCamDist->clear();
  m_ui->lneVtCenterHeight->clear();
  m_ui->lneCamHeight->clear();
  m_ui->lneCamOffset->clear();

  //clear the results window since they are now stale
  m_ui->lstResults->clear();
}
QStringList MainWindow::calcLocation(const TriangleInfo& params)
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: calcHeight()" << endl;
    cout << "DEBUG: MainWindow: paramaters used for calculation" << endl;
    cout << params.toString("\t") << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  const auto tgtSize = params.tgtSizeV / 2.0;

  //calculate min camera height
  const auto minTanResult = std::tan((params.vFov / 2) + params.camPitch);
  auto minCamH = -params.minDist * minTanResult + params.tgtHeight - tgtSize;

  //calculate max camera height
  const auto maxTanResult = std::tan((params.vFov / 2) - params.camPitch);
  auto maxCamH = params.minDist * maxTanResult + params.tgtHeight + tgtSize;

  QStringList toReturn;
  toReturn.push_back("max: " + QString::number(maxCamH));
  toReturn.push_back("min: " + QString::number(minCamH));
  return toReturn;
}
QStringList MainWindow::calcDistance(const TriangleInfo& params)
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: calcDistance()" << endl;
    cout << "DEBUG: MainWindow: paramaters used for calculation" << endl;
    cout << params.toString("\t") << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  //Vc: Height to the center of the vision target
  //Hc: Height to the center of the camera
  //Vh: Height of the vision target
  //phi: vertical FOV of the camera
  //theta: pitch of the camera
  //max = (Hc - Vc - (Vh / 2)) / (tan((phi / 2) - theta))
  //min = (Vc - (Vh / 2) - Hc) / (tan((phi / 2) + theta))

  auto Vc = params.tgtHeight;
  auto Hc = params.camHeight;
  auto Vh = params.tgtSizeV;
  auto phi = params.vFov;
  auto theta = params.camPitch;

  auto max = (Hc - Vc - (Vh / 2)) / (std::tan((phi / 2) - theta));
  auto min = (Vc - (Vh / 2) - Hc) / (std::tan((phi / 2) + theta));

  QStringList toReturn;
  toReturn.push_back("max: " + QString::number(max));
  toReturn.push_back("min: " + QString::number(min));
  return toReturn;
}
QStringList MainWindow::calcFov(const TriangleInfo& params)
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: calcFov()" << endl;
    cout << "DEBUG: MainWindow: paramaters used for calculation" << endl;
    cout << params.toString("\t") << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  return QStringList();
}
QStringList MainWindow::calcOrientation(const TriangleInfo& params)
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: calcPitch()" << endl;
    cout << "DEBUG: MainWindow: paramaters used for calculation" << endl;
    cout << params.toString("\t") << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  return QStringList();
}
void MainWindow::displayAbout()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: displayAbout()" << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  auto author = QStringLiteral("Author: Matthew \"Cheeto\" Russell");
  auto title = QStringLiteral("Camera Calculator");
  auto description = QStringLiteral("Tool used to calculate camera placement parameters");
  auto version = QStringLiteral("Version: 0.1.0");
  auto contactInfo = QStringLiteral("email: mprussell@robobees.org");

  QMessageBox::about(this, "About FRC Score Analyzer", title + "\n" + version + "\n" +
                     description + "\n" + author + "\n" + contactInfo);
}
bool MainWindow::verifyInput()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: verifyInput()" << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  //verify that all the vision target info is provided
  if (m_ui->lneVtWidth->text().isEmpty() || m_ui->lneVtHeight->text().isEmpty() || 
      m_ui->lneVtCenterHeight->text().isEmpty())
  {
    if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::ERRORS_ONLY)
    {
      cout << "ERROR: MainWindow: all info for the vision target must be provided" << endl;
    } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::ERRORS_ONLY)
    return false;
  }//end if (m_ui->lneVtWidth->text().isEmpty() || m_ui->lneVtHeight->text().isEmpty() || 

  //figure out which fields are empty
  int numEmpty = 0;
  if (m_ui->lneCamDist->text().isEmpty())
  {
    if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::VERBOSE_INFO)
    {
      cout << "INFO: MainWindow: distance is empty" << endl;
    } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::VERBOSE_INFO)
    numEmpty++;
  } //end  if (m_ui->lneCamDist->text().isEmpty())
  if(m_ui->lneCamPitch->text().isEmpty() || m_ui->lneCamYaw->text().isEmpty())
  {
    if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::VERBOSE_INFO)
    {
      cout << "INFO: MainWindow: orientation is empty" << endl;
    } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::VERBOSE_INFO)
    numEmpty++;
  } //end  if(m_ui->lneCamPitch->text().isEmpty() || m_ui->lneCamYaw->text().isEmpty())
  if (m_ui->lneCamFovVert->text().isEmpty() || m_ui->lneCamFovHor->text().isEmpty())
  {
    if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::VERBOSE_INFO)
    {
      cout << "INFO: MainWindow: FOV is empty" << endl;
    } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::VERBOSE_INFO)
    numEmpty++;
  }//end if (m_ui->lneCamFovVert->text().isEmpty()||m_ui->lneCamFovHor->text().isEmpty())
  if (m_ui->lneCamHeight->text().isEmpty() || m_ui->lneCamOffset->text().isEmpty())
  {
    if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::VERBOSE_INFO)
    {
      cout << "INFO: MainWindow: Position is empty" << endl;
    } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::VERBOSE_INFO)
    numEmpty++;
  }//end if (m_ui->lneCamHeight->text().isEmpty() || m_ui->lneCamOffset->text().isEmpty())

  //system is underdefined, can't calculate with missing information
  if (numEmpty > 1)
  {
    if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::ERRORS_ONLY)
    {
      cout << "ERROR: MainWindow: not enought info provided for calculations" << endl;
      cout << "\t" << numEmpty << " blocks are empty, only 1 can be" << endl;
    } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::ERRORS_ONLY)
    return false;
  } //end  if (numEmpty > 1)
  
  //system is overdefined, nothing to calculate
  if (numEmpty == 0)
  {
    if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::ERRORS_ONLY)
    {
      cout << "ERROR: MainWindow: All fields are defined, nothing to calculate" << endl;
    } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::ERRORS_ONLY)
    return false;
  } //end  if (numEmpty == 0)
  return true;
}

//event handlers
void MainWindow::closeEvent(QCloseEvent* e)
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: closeEvent()" << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
}

//constructors
MainWindow::MainWindow()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: constructing..." << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  m_ui = std::make_unique<Ui_MainWindow>();
  m_ui->setupUi(this);

  makeConnections();
  setup();
}
MainWindow::~MainWindow()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: destructing..." << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
}

//public slots
void MainWindow::btnResetClickHandler()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: btnResetClickHandler()" << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  reset();
}
void MainWindow::btnCalculateClickHandler()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: btnCalculateClickHandler()" << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  if (!verifyInput())
  {
    if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::ERRORS_ONLY)
    {
      cout << "ERROR: output cannot be calculated due to invalid inputs" << endl;
    } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::ERRORS_ONLY)
    return;
  } //end  if (!verifyInput())

  //get parameters
  auto dist = m_ui->lneCamDist->text().toDouble();
  auto camHeight = m_ui->lneCamHeight->text().toDouble();
  auto camOffset = m_ui->lneCamOffset->text().toDouble();
  auto vFov = m_ui->lneCamFovVert->text().toDouble() * DEG_TO_RAD;
  auto hFov = m_ui->lneCamFovHor->text().toDouble() * DEG_TO_RAD;
  auto camPitch = m_ui->lneCamPitch->text().toDouble() * DEG_TO_RAD;
  auto camYaw = m_ui->lneCamYaw->text().toDouble() * DEG_TO_RAD;
  auto tgtHeight = m_ui->lneVtCenterHeight->text().toDouble();
  auto tgtSizeV = m_ui->lneVtHeight->text().toDouble();
  auto tgtSizeH = m_ui->lneVtWidth->text().toDouble();

  auto params = TriangleInfo(dist, camHeight, camOffset, vFov, hFov, camPitch, camYaw,
                             tgtHeight, tgtSizeV, tgtSizeH);

  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: values at time of button press" << endl;
    cout << params.toString("\t") << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)

  QStringList results;
  //determine which calculation needs made
  //TODO: figure out better way to do this
  if (m_ui->lneCamHeight->text().isEmpty() || m_ui->lneCamOffset->text().isEmpty())
  {
    results = calcLocation(params);
  }//end if (m_ui->lneCamHeight->text().isEmpty() || m_ui->lneCamOffset->text().isEmpty())
  else if (m_ui->lneCamDist->text().isEmpty())
  {
    results = calcDistance(params);
  } //end  else if (m_ui->lneCamDist->text().trimmed().isEmpty())
  else if (m_ui->lneCamFovVert->text().isEmpty() || m_ui->lneCamFovHor->text().isEmpty())
  {
    results = calcFov(params);
  }//end else if(m_ui->lneCamFovVert->text.isEmpty()||m_ui->lneCamFovHor->text.isEmpty())
  else if (m_ui->lneCamPitch->text().isEmpty() || m_ui->lneCamYaw->text().isEmpty())
  {
    results = calcOrientation(params);
  }//end else if(m_ui->lneCamPitch->text().isEmpty() || m_ui->lneCamYaw->text().isEmpty())

  for (auto line : results)
  {
    m_ui->lstResults->insertItem(0, line);
  } //end  for (auto line : results)
}
void MainWindow::actAboutHandler()
{
  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  {
    cout << "DEBUG: MainWindow: actAboutHandler()" << endl;
  } //end  if (CmdOptions::verbosity >= CmdOptions::VERBOSITY::DEBUG_INFO)
  displayAbout();
}

