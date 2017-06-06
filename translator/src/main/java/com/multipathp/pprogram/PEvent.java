package com.multipathp.pprogram;

import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import java.util.Objects;
import java.util.Optional;

@FreeBuilder
public abstract class PEvent {
    public static PEvent EVENT_NULL = new PEvent.Builder().setName("$EVENT_NULL$").build();
    abstract String getName();
    abstract Optional<PType> getPayloadType();

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof PEvent)) {
            return false;
        }
        PEvent other = (PEvent) o;
        return Objects.equals(getName(), other.getName());
    }

    @Override
    public int hashCode() {
        return Objects.hash(getName());
    }

    static class Builder extends PEvent_Builder {

    }
}