export type Activitate = {
  id: string;
  title: string;
  description: string;
  category: string;
};

export const activitati: Activitate[] = [
  {
    id: '1',
    title: 'Alergare',
    description: '30 de minute în parc',
    category: 'Sport',
  },
  {
    id: '2',
    title: 'Citit',
    description: '20 de pagini dintr-o carte de dezvoltare personală',
    category: 'Dezvoltare',
  },
  {
    id: '3',
    title: 'Proiect PAAM',
    description: 'Finalizarea laboratorului 3',
    category: 'Facultate',
  },
];