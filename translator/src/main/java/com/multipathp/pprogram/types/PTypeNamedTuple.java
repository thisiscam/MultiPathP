package com.multipathp.pprogram.types;

import java.util.LinkedHashMap;
import java.util.Objects;

public class PTypeNamedTuple extends PType implements IPTypeGetIndexable {
    LinkedHashMap<String, PType> namedTypes;

    public PTypeNamedTuple(LinkedHashMap<String, PType> namedTypes) {
        super("$namedtuple$", true);
        this.namedTypes = namedTypes;
    }

    public PType getTypeByAttribute(String name) {
        return namedTypes.get(name);
    }

    @Override
    public PType getIndexedInnerType(int index) {
        return (PType) namedTypes.values().toArray()[index];
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof PTypeNamedTuple)) {
            return false;
        }
        PTypeNamedTuple other = (PTypeNamedTuple) o;
        return Objects.equals(namedTypes, other.namedTypes);
    }

    @Override
    public int hashCode() {
        return Objects.hash(namedTypes, super.hashCode());
    }
}