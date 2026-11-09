#pragma once
class QWidget;
namespace M3Ui {
// Install once on real top-level windows. Native frames keep native resizing;
// custom frames receive eight small edge/corner handles.
void enableWindowResize(QWidget *window);
}
