//
// C++ Interface: ItemView
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ITEMVIEW_H
#define ITEMVIEW_H

#include <QAbstractItemView>
#include <QGraphicsView>
#include <QPersistentModelIndex>
#include <QDebug>
#include <QResizeEvent>

#include "GraphicsScene.h"
#include "Model.h"

/**
 @author Joachim Schiele <js@lastlog.de>
*/
class ItemView : public QAbstractItemView {
  public:
    ItemView( QGraphicsView* view, Model *model );
    ~ItemView();
  private:
    QGraphicsView* view;
    GraphicsScene* scene;
    Model *model;
    QRect visualRect( const QModelIndex &index ) const ;
    void scrollTo( const QModelIndex &index, ScrollHint hint = EnsureVisible ) ;
    QModelIndex indexAt( const QPoint &point ) const ;
    QModelIndex moveCursor( CursorAction cursorAction, Qt::KeyboardModifiers modifiers ) ;
    int horizontalOffset() const ;
    int verticalOffset() const ;
    bool isIndexHidden( const QModelIndex &index ) const ;
    void setSelection( const QRect &rect, QItemSelectionModel::SelectionFlags command ) ;
    QRegion visualRegionForSelection( const QItemSelection &selection ) const ;
    void processNewNodes();

    void rowsInserted( const QModelIndex & parent, int start, int end );
    void rowsAboutToBeRemoved( const QModelIndex & parent, int start, int end );
    void dataChanged( const QModelIndex & , const QModelIndex & );
    void reset();
};

#endif
