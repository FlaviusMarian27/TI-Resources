import { useNavigation, useRouter } from 'expo-router';
import { ComponentRef, useEffect, useRef, useState } from 'react';
import { Button, ScrollView, StyleSheet, Text, TextInput, View } from 'react-native';

type ActivitateSalvata = {
  titlu: string;
  descriere: string;
};

export default function AdaugaActivitate() {
  const navigation = useNavigation();
  const router = useRouter();
  const titluRef = useRef<ComponentRef<typeof TextInput>>(null);

  const [titlu, setTitlu] = useState('');
  const [descriere, setDescriere] = useState('');
  const [eroare, setEroare] = useState('');
  const [salvat, setSalvat] = useState<ActivitateSalvata | null>(null);

  // Titlul ecranului urmărește textul din câmpul „Titlu”
  useEffect(() => {
    navigation.setOptions({
      title: titlu.trim() !== '' ? titlu : 'Adaugă activitate',
    });
  }, [titlu, navigation]);

  const salveaza = () => {
    if (titlu.trim() === '') {
      setEroare('Titlul activității nu poate fi gol.');
      setSalvat(null);
      return;
    }
    setEroare('');
    setSalvat({ titlu: titlu.trim(), descriere: descriere.trim() });
  };

  const reseteaza = () => {
    setTitlu('');
    setDescriere('');
    setEroare('');
    setSalvat(null);
  };

  const editeazaTitlul = () => {
    titluRef.current?.focus();
  };

  const anuleaza = () => {
    if (router.canGoBack()) {
      router.back();
    } else {
      router.replace('/');
    }
  };

  return (
    <ScrollView contentContainerStyle={styles.container}>
      <Text style={styles.eticheta}>Titlu</Text>
      <TextInput
        ref={titluRef}
        style={[styles.input, eroare !== '' && styles.inputEroare]}
        value={titlu}
        onChangeText={setTitlu}
        placeholder="Ex: Alergare"
      />
      {eroare !== '' && <Text style={styles.eroare}>{eroare}</Text>}

      <Text style={styles.eticheta}>Descriere</Text>
      <TextInput
        style={[styles.input, styles.inputMare]}
        value={descriere}
        onChangeText={setDescriere}
        placeholder="Ex: 30 de minute în parc"
        multiline
      />

      <Button title="Salvează" onPress={salveaza} />
      <Button title="Editează titlul" onPress={editeazaTitlul} />
      <Button title="Resetează" onPress={reseteaza} />
      <Button title="Anulează" onPress={anuleaza} />

      {salvat && (
        <View style={styles.rezultat}>
          <Text style={styles.rezultatTitlu}>Activitate introdusă</Text>
          <Text style={styles.rezultatText}>Titlu: {salvat.titlu}</Text>
          <Text style={styles.rezultatText}>
            Descriere: {salvat.descriere !== '' ? salvat.descriere : '(fără descriere)'}
          </Text>
        </View>
      )}
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    padding: 16,
    gap: 12,
  },
  eticheta: {
    fontSize: 14,
    fontWeight: '600',
  },
  input: {
    borderWidth: 1,
    borderColor: '#ccc',
    borderRadius: 8,
    padding: 10,
    fontSize: 16,
    backgroundColor: '#fff',
  },
  inputMare: {
    minHeight: 80,
    textAlignVertical: 'top',
  },
  inputEroare: {
    borderColor: '#dc2626',
  },
  eroare: {
    color: '#dc2626',
    fontSize: 14,
  },
  rezultat: {
    backgroundColor: '#f2f4f8',
    borderRadius: 12,
    padding: 16,
    gap: 4,
  },
  rezultatTitlu: {
    fontSize: 18,
    fontWeight: 'bold',
  },
  rezultatText: {
    fontSize: 16,
    color: '#555',
  },
});