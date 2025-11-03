<template>
	<form @submit.prevent>
		<h4>Add Category</h4>
		<simple-input-box 
			v-model="category.name"
			class="update-input" 
			type="text" 
			placeholder="Category Name"
		/>
		<small-button 
			class="update-button" 
			@click="addCategory"
		>
			Add
		</small-button>
	</form>
</template>

<script>
export default {
	name: 'add-category-form',
	data() {
		return {
			category: {
				name: '',
				items: 0,
				removed: false
			}
		}
	},
	methods: {
		addCategory() {
			this.$emit('add-category', this.category)
			this.category = {
				name: '',
				items: 0,
				removed: false
			}
		}
	}
};
</script>

<style scoped>
form{
	display: flex;
	flex-direction: column;
}
.update-input{
	width: 100%;
	margin-top: 10px;
}
.update-button{
	align-self: flex-end;
	margin-top: 10px;
}
</style>