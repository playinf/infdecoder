/*
 * chart.h
 *
 *
 * author: Playinf
 * playinf@stu.xmu.edu.cn
 *
 */
#ifndef __CHART_H__
#define __CHART_H__

#include <infdef.h>
#include <chart_cell.h>

class chart {
public:
    typedef unsigned int size_type;

    chart(size_type len);
    ~chart();

    chart_cell* get_cell(size_type start_pos, size_type end_pos) const;
private:
    chart_cell** table;
    size_type size;
};

#endif /* __CHART_H__ */
