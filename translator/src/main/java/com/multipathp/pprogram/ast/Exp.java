package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.types.PType;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
public abstract class Exp extends PASTNode {
    public abstract PType getExpressionType();
}
