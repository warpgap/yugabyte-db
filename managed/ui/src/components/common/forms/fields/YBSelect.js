// Copyright (c) YugaByte, Inc.

import React, { Component } from 'react';
import { isValidFunction } from 'utils/ObjectUtils';
import { YBLabel } from 'components/common/descriptors';

// TODO: Rename to YBSelect after changing prior YBSelect references.
// TODO: Make default export after checking all corresponding imports.

export class YBControlledSelect extends Component {
  render() {
    const {selectVal, input, options, onInputChanged} = this.props;
    return (
      <select {...input} className="form-control" onChange={onInputChanged} value={selectVal}>
        {options}
      </select>
    )
  }
}

export class YBUnControlledSelect extends Component {
  render() {
    const { input, options, onInputChanged, readOnlySelect } = this.props;

    function onChange(event) {
      input.onChange(event.target.value);
      if (isValidFunction(onInputChanged)) {
        onInputChanged(event.target.value);
      }
    }

    return (
      <select {...input} className="form-control" disabled={readOnlySelect} onChange={onChange}>
        {options}
      </select>
    );
  }
}

export class YBControlledSelectWithLabel extends Component {
  render() {
    const { label, meta, ...otherProps} = this.props;
    return (
      <YBLabel label={label} meta={meta}>
        <YBControlledSelect {...otherProps}/>
      </YBLabel>
    );
  }
}

export default class YBSelectWithLabel extends Component {
  render() {
    const { label, meta, ...otherProps} = this.props;
    return (
      <YBLabel label={label} meta={meta}>
        <YBUnControlledSelect {...otherProps} />
      </YBLabel>
    );
  }
}

// TODO: Rename all prior YBSelect references to YBSelectWithLabel.
export var YBSelect = YBSelectWithLabel;