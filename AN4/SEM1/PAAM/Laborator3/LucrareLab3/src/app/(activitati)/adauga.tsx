import { Button, StyleSheet, Text, View } from 'react-native';
import { useRouter } from 'expo-router';

export default function AdaugaActivitate() {
  const router = useRouter();

  const anuleaza = () => {
    if (router.canGoBack()) {
      router.back();
    } else {
      router.replace('/');
    }
  };

  return (
    <View style={styles.container}>
      <Text style={styles.text}>
        Aici va apărea formularul pentru adăugarea unei activități noi. Deocamdată
        ecranul are doar rol demonstrativ.
      </Text>
      <Button title="Anulează" onPress={anuleaza} />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    padding: 16,
    gap: 16,
  },
  text: {
    fontSize: 16,
    color: '#555',
  },
});