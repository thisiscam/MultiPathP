package com.multipathp.pprogram.ast;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
public abstract class UnaryExp extends Exp {
    public abstract Exp getExpression();

    @Override
    public final int getChildrenCount() {
        return 1;
    }

    @Override
    public final PASTNode getChild(int i) {
        if(i == 0) {
            return getExpression();
        } else {
            throw new IndexOutOfBoundsException();
        }
    }
}
