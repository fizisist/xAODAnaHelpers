<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [xAODAnaHelpers (xAH)](#xaodanahelpers-xah)
  - [Current Working Version](#current-working-version)
  - [Migrating](#migrating)
    - [From 00-00-04 to 00-00-05](#from-00-00-04-to-00-00-05)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# xAODAnaHelpers (xAH)

The xAOD analysis framework, born out of ProofAna.

## Current Working Version

This version uses AB 2.3.12.

## Migrating

### From 00-00-04 to 00-00-05

The constructors for the algorithms have all been changed to default constructors. We have also centralized a lot of code so that `EL::Algorithm` is replaced by `xAH::Algorithm` where possible. For updating constructors in your code, replace

```
BasicEventSelection* baseEventSel = new BasicEventSelection(  "baseEventSel", localDataDir+"baseEvent.config");
```

with

```
BasicEventSelection* baseEventSel             = new BasicEventSelection();
baseEventSel->setName("baseEventSel")->setConfig(localDataDir+"baseEvent.config");
```

See [xAODAnaHelpers/Algorithm.h](xAODAnaHelpers/Algorithm.h) for more details on the uniform constructors.
