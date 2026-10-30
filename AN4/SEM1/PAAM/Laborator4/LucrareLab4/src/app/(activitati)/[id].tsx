import { Button, StyleSheet, Text, View } from 'react-native';
import { useLocalSearchParams, useRouter } from 'expo-router';
import { activitati } from '../../data/activitati';

export default function DetaliiActivitate() {
  const { id } = useLocalSearchParams<{ id: string }>();
  const router = useRouter();

  const activitate = activitati.find((a) => a.id === id);

  const inapoi = () => {
    if (router.canGoBack()) {
      router.back();
    } else {
      router.replace('/');
    }
  };

  if (!activitate) {
    return (
      <View style={styles.container}>
        <Text style={styles.titlu}>Activitatea nu există</Text>
        <Text style={styles.text}>Nu am găsit nicio activitate cu id-ul „{id}”.</Text>
        <Button title="Înapoi la activități" onPress={() => router.replace('/')} />
      </View>
    );
  }

  return (
    <View style={styles.container}>
      <Text style={styles.titlu}>{activitate.title}</Text>
      <Text style={styles.text}>{activitate.description}</Text>
      <Text style={styles.categorie}>{activitate.category}</Text>
      <Button title="Înapoi la activități" onPress={inapoi} />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    padding: 16,
    gap: 12,
  },
  titlu: {
    fontSize: 24,
    fontWeight: 'bold',
  },
  text: {
    fontSize: 16,
    color: '#555',
  },
  categorie: {
    fontSize: 14,
    fontWeight: '600',
    color: '#2563eb',
  },
});