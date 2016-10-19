#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include <QAbstractItemModel>
#include <QApplication>
#include <deque>
#include <memory>

template<class _Ty, class... _Types> std::shared_ptr<_Ty> make_shared_nothrow(_Types&&... _Args)
{
    std::shared_ptr<_Ty> res;

    std::_Ref_count_obj<_Ty>* obj = new(std::nothrow) std::_Ref_count_obj<_Ty>(std::forward<_Types>(_Args)...);
    if (!obj)
        return res;

    res._Resetp0(obj->_Getptr(), obj);
    return (res);
}
