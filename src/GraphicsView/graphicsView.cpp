/*
    This file is part of automate.
      Copyright Joachim Schiele

    automate is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    automate is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with automate.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
  @author Joachim Schiele <js@lastlog.de>
*/

#include "graphicsView.h"

// HINT: this has nothing to do with a QGraphicsView (we don't inherit it)
GraphicsView::GraphicsView ( Model *model, QMainWindow* parent ) : AbstractView ( parent ) {
    this->model = model;
    sb = new QStatusBar;
    view = new QGraphicsView ( this );
    setWindowIcon ( QIcon ( QString::fromUtf8 ( ":/icons/view_choose.png" ) ) );

    //QGLWidget* view = new QGLWidget ;
    scene = new GraphicsScene ( model, this );
    itemView = new ItemView ( view, scene, model, this );

    // we hide this widget since we only abuse it's connectivity for our QGraphicsScene/View
    itemView->hide();

    tb = new QToolBar ( "title", this );
    populateMenu();
    tb->setMovable ( true );

    setCentralWidget ( view );
    addToolBar ( Qt::LeftToolBarArea, tb );
    setStatusBar ( sb );

    // this selectionModel can be used if one want's to have the same selection in all
    // views, however the QGraphicsView(QGraphicsScene) will need some adaptions first
    // to make the TreeView select the same items as the GraphicsView and vice versa.
//     selectionModel = new QItemSelectionModel ( model );
//     itemView->setSelectionModel ( selectionModel );

    //FIXME i need to do something about this
    resize ( 800, 600 );
    view->setSceneRect(QRect(0,0,500,500));
    scene->setSceneRect(QRect(0,0,500,500));

    connect ( scene,SIGNAL ( zoomOut() ), this, SLOT ( zoomOut() ) );
    connect ( scene,SIGNAL ( zoomIn() ), this, SLOT ( zoomIn() ) );
    connect ( scene,SIGNAL ( zoomFit() ), this, SLOT ( zoomFit() ) );
    connect ( scene, SIGNAL ( hideView() ), this, SLOT ( hide() ) );
    connect ( scene, SIGNAL ( toggleRenderHints() ), itemView, SLOT ( toggleRenderHints() ) );
}

GraphicsView::~GraphicsView() {
//   qDebug() << __PRETTY_FUNCTION__;
//     delete selectionModel;
    delete tb;
    delete itemView;
    delete scene;
    delete view;
    delete sb;
}

void GraphicsView::populateMenu() {
    QAction* toggleStartAction = new QAction ( QIcon ( ":/icons/startNode.png" ),"'s' - start toggle", this );
    tb->addAction ( toggleStartAction );
    connect ( toggleStartAction , SIGNAL ( triggered() ), scene, SLOT ( toggleStartEvent() ) );

    QAction* toggleFinalAction = new QAction ( QIcon ( ":/icons/finalNode.png" ),"'f' - final toggle", this );
    tb->addAction ( toggleFinalAction );
    connect ( toggleFinalAction, SIGNAL ( triggered() ), scene, SLOT ( toggleFinalEvent() ) );

    tb->addSeparator();

    QAction* insertNodeAction = new QAction ( QIcon ( ":/icons/bookmark_add.png" ),"'n' - insert node", this );
    tb->addAction ( insertNodeAction );
    connect ( insertNodeAction , SIGNAL ( triggered() ), this, SLOT ( insertNode() ) );
    connect ( this, SIGNAL ( insertNodeSignal ( QPoint ) ), scene, SLOT ( insertNode ( QPoint ) ) );

    QAction* removeAction = new QAction ( QIcon ( ":/icons/button_cancel.png" ),"'x' - del node/connection", this );
    tb->addAction ( removeAction );
    connect ( removeAction , SIGNAL ( triggered() ), scene, SLOT ( removeEvent() ) );

    tb->addSeparator();

    QAction* printAction = new QAction ( QIcon ( ":/icons/printer2.png" ),"'p' - Print", this );
    tb->addAction ( printAction );
    connect ( printAction, SIGNAL ( triggered() ), scene, SLOT ( print() ) );

    tb->addSeparator();

    QAction* zoomInAction = new QAction ( QIcon ( ":/icons/viewmag+.png" ),"Zoom In '+'", this );
    tb->addAction ( zoomInAction );
    connect ( zoomInAction, SIGNAL ( triggered() ), this, SLOT ( zoomIn() ) );

    QAction* zoomOutAction = new QAction ( QIcon ( ":/icons/viewmag-.png" ),"Zoom Out '-'", this );
    tb->addAction ( zoomOutAction );
    connect ( zoomOutAction, SIGNAL ( triggered() ), this, SLOT ( zoomOut() ) );

    QAction* zoomNormalAction = new QAction ( QIcon ( ":/icons/viewmag1.png" ),"Zoom Normal", this );
    tb->addAction ( zoomNormalAction );
    connect ( zoomNormalAction, SIGNAL ( triggered() ), this, SLOT ( zoomNormal() ) );

    QAction* zoomFitAction = new QAction ( QIcon ( ":/icons/viewmagfit.png" ),"Zoom Fit '/'", this );
    tb->addAction ( zoomFitAction );
    connect ( zoomFitAction, SIGNAL ( triggered() ), this, SLOT ( zoomFit() ) );

    tb->addSeparator();

    QAction* toggleHighlightAction = new QAction ( QIcon ( ":/icons/highlighting.png" ),"'h' - toggleHighlightAction", this );
    tb->addAction ( toggleHighlightAction );
    connect ( toggleHighlightAction, SIGNAL ( triggered() ), scene, SLOT ( toggleHighlight() ) );

    QAction* toggleAntialiasingAction = new QAction ( QIcon ( ":/icons/antialiasing.png" ),"'t' - toggleAntialiasingAction", this );
    tb->addAction ( toggleAntialiasingAction );
    connect ( toggleAntialiasingAction, SIGNAL ( triggered() ), itemView, SLOT ( toggleRenderHints() ) );

    QAction* toggleBoundingBoxAction = new QAction ( QIcon ( ":/icons/boundingBox.png" ),"'b' - toggleBoundingBoxAction", this );
    tb->addAction ( toggleBoundingBoxAction );
    connect ( toggleBoundingBoxAction, SIGNAL ( triggered() ), scene, SLOT ( toggleBoundingBox() ) );

    QAction* toggleDrawItemShapeAction = new QAction ( QIcon ( ":/icons/drawShape.png" ),"'e' - toggleDrawItemShapeAction ", this );
    tb->addAction ( toggleDrawItemShapeAction );
    connect ( toggleDrawItemShapeAction, SIGNAL ( triggered() ), scene, SLOT ( toggleDrawItemShape() ) );

    QAction* toggleColoredConnectionHelper = new QAction ( QIcon ( ":/icons/coloredConnectionHelper.png" ),"'l' - toggleColoredConnectionHelper ", this );
    tb->addAction ( toggleColoredConnectionHelper );
    connect ( toggleColoredConnectionHelper, SIGNAL ( triggered() ), scene, SLOT ( toggle_coloredConnectionHelper() ) );

    tb->addSeparator();

    QAction* toggle_customNodeLabelsAction = new QAction ( QIcon ( ":/icons/customNodeLabel.png" ),"'?' - toggle_customNodeLabelsAction ", this );
    tb->addAction ( toggle_customNodeLabelsAction );
    connect ( toggle_customNodeLabelsAction, SIGNAL ( triggered() ), scene, SLOT ( toggle_customNodeLabels() ) );

    QAction* toggle_customConnectionLabelsAction = new QAction ( QIcon ( ":/icons/customConnectionLabel.png" ),"'?' - toggle_customConnectionLabelsAction ", this );
    tb->addAction ( toggle_customConnectionLabelsAction );
    connect ( toggle_customConnectionLabelsAction, SIGNAL ( triggered() ), scene, SLOT ( toggle_customConnectionLabels() ) );
}

void GraphicsView::zoomIn() {
    view->scale ( 1.2, 1.2 );
}

void GraphicsView::zoomOut() {
    view->scale ( 0.8, 0.8 );
}

void GraphicsView::zoomNormal() {
    view->resetMatrix();
}

void GraphicsView::insertNode() {
    QPoint p ( qrand() %500,qrand() %200 );
    emit insertNodeSignal ( p );
}

void GraphicsView::zoomFit() {
    qDebug() << "FIXME zoomFit() not implemented yet";
    //FIXME put your code here
//   view->resetMatrix();
//   view->setSceneRect(scene->sceneRect());
}

QPoint GraphicsView::queryMouseCoordinatesOverQGraphicsView() {
    QPointF pos = view->mapToScene ( view->mapFromGlobal ( QCursor::pos() ) );
    return pos.toPoint();
}
