package com.multipathp.pprogram.ast;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
public abstract class LiteralExp extends Exp {
    @Override
    public final int getChildrenCount() {
        return 0;
    }

    @Override
    public final PASTNode getChild(int i) {
        throw new IndexOutOfBoundsException();
    }
}
