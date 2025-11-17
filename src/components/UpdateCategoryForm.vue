<template>
	<form @submit.prevent>
		<h4>{{ actionType === 'add' ? 'New category' : 'Edit category' }}</h4>
		<simple-input-box 
			v-model="formCategory.name"
			class="update-input" 
			type="text" 
			placeholder="Category Name"
		/>
		<small-button 
			class="update-button" 
			@click="actionType === 'add' ? addCategory() : updateCategory()"
		>
			{{ actionType === 'add' ? 'Add' : 'Update' }}
		</small-button>
	</form>
</template>

<script>
export default {
	name: 'update-category-form',
	data() {
		return {
			formCategory: {
				name: '',
				items: [],
				removed: false
			}
		}
	},
	props: {
		actionType: {
			type: String,
			required: true
		},
		category: {
			type: Object,
			default: () => ({name: '', items: [], removed: false})
		}
	},
	watch: {
		category: {
			handler(newVal) {
				this.formCategory = newVal
			},
			immediate: true
		}
	},
	methods: {
		addCategory() {
			this.$emit('add-category', this.formCategory)
			this.formCategory = {
				name: '',
				items: [],
				removed: false
			}
		},
		updateCategory() {
			this.$emit('update-category', this.formCategory)
			this.formCategory = {
				name: '',
				items: [],
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