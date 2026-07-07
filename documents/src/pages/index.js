import React from 'react';
import {Redirect} from '@docusaurus/router';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';

export default function Home() {
  const {i18n} = useDocusaurusContext();
  const prefix = i18n.currentLocale === i18n.defaultLocale ? '' : `/${i18n.currentLocale}`;
  return <Redirect to={`${prefix}/guide/intro`} />;
}
