package com.multipathp.pprogram.ast;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by jianqiaoyang on 6/1/17.
 */
public abstract class PASTNode {
    public abstract <T> T accept(PASTVisitor<T> visitor);

    public abstract int getChildrenCount();
    public abstract PASTNode getChild(int i);

    private Map<Class, PASTAnnotation> annotationMap = new HashMap<>();
    public <T extends PASTAnnotation> void decorate(T annotation) {
        if(annotationMap.containsKey(annotation.getClass())) {
            throw new IllegalArgumentException("tree already decorated with this type of annotation");
        }
        annotationMap.put(annotation.getClass(), annotation);
    }

    @SuppressWarnings("unchecked")
    public <T extends PASTAnnotation> T getDecoration(Class<T> claz) {
        return (T) annotationMap.get(claz);
    }
}
