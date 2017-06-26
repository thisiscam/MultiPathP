package com.multipathp.pprogram.types;

import com.multipathp.pprogram.ast.PTransition;

import java.util.Objects;

public class PType {
    public static PType NULL = new PType("$null$", false);
    public static PType EVENT = new PType("$event$", false);
    public static PType INT = new PType("$int$", false);
    public static PType BOOL = new PType("$bool$", false);
    public static PType MACHINE = new PType("$machine$", false);
    public static PType VOID = new PType("$void$", false);
    public static PType ANY = new PType("$any$", true);
    private String typename;
    private boolean clonable;

    public String getName() {
        return typename;
    }

    public boolean getIsVoid() {
        return this == VOID;
    }

    PType(String typename, boolean clonable) {
        this.typename = typename;
        this.clonable = clonable;
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof PType)) {
            return false;
        }
        PType other = (PType) o;
        return Objects.equals(typename, other.typename);
    }

    @Override
    public int hashCode() {
        return Objects.hash(typename, clonable);
    }
}